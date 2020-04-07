package com.angelsware.http;

import android.os.AsyncTask;

import com.angelsware.engine.AppActivity;

import java.net.URL;
import java.net.HttpURLConnection;
import java.net.SocketTimeoutException;
import java.net.MalformedURLException;
import java.net.UnknownHostException;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.OutputStream;
import java.util.Dictionary;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

class Request extends AsyncTask<String, Integer, String> {
	private enum RequestResult {
		OK(0),
		UNKNOWN_HOST(1),
		MALFORMED_URL(2),
		FILE_NOT_FOUND(3),
		SOCKET_TIMEOUT(4),
		ERROR(5);

		RequestResult(int value) {
		}
	}

	public static native void onProgress(int bytesRead, int length);
	public static native void onComplete(int result, int responseCode, byte[] data);

	public void setRequestMethod(String requestMethod) {
		mRequestMethod = requestMethod;
	}

	public void setData(byte[] data, int size) {
		mContent = data;
		mContentLength = size;
	}

	public void addHeader(String key, String value) {
		mHeaders.put(key, value);
	}

	public void send(String uri) {
		execute(uri);
	}

	@Override
	protected String doInBackground(String... uri) {
		try {
			URL url = new URL(uri[0]);
			HttpURLConnection connection = (HttpURLConnection)url.openConnection();
			connection.setRequestMethod(mRequestMethod);

			Iterator it = mHeaders.entrySet().iterator();
			while (it.hasNext()) {
				Map.Entry pair = (Map.Entry)it.next();
				connection.setRequestProperty(pair.getKey().toString(), pair.getValue().toString());
				it.remove();
			}

			if (mContentLength > 0 && mContent != null) {
				connection.setDoOutput(true);
				connection.setFixedLengthStreamingMode(mContentLength);
				OutputStream os = connection.getOutputStream();
				os.write(mContent, 0, mContentLength);
				os.close();
			}

			connection.connect();
			int length = connection.getContentLength();

			mResponseCode = connection.getResponseCode();
			InputStream input;
			if (mResponseCode < HttpURLConnection.HTTP_BAD_REQUEST) {
				input = new BufferedInputStream(connection.getInputStream(), 8192);
			} else {
				input = new BufferedInputStream(connection.getErrorStream(), 8192);
			}

			mByteArrayOutputStream = new ByteArrayOutputStream();
			byte[] data = new byte[1024];
			int total = 0;
			int count;
			while ((count = input.read(data)) != -1) {
				total += count;
				mByteArrayOutputStream.write(data, 0, count);
				publishProgress(total, length);
			}
			mByteArrayOutputStream.flush();
			mByteArrayOutputStream.close();
			input.close();
		} catch (UnknownHostException e) {
			onError(RequestResult.UNKNOWN_HOST);
		} catch (MalformedURLException e) {
			onError(RequestResult.MALFORMED_URL);
		} catch (FileNotFoundException e) {
			onError(RequestResult.FILE_NOT_FOUND);
		} catch (SocketTimeoutException e) {
			onError(RequestResult.SOCKET_TIMEOUT);
		} catch (Exception e) {
			onError(RequestResult.ERROR);
		}
		return null;
	}

	@Override
	protected void onProgressUpdate(Integer... progress) {
		onProgress(progress[0], progress[1]);
	}

	@Override
	protected void onPostExecute(String url) {
		if (mByteArrayOutputStream != null) {
			onComplete(RequestResult.OK.ordinal(), mResponseCode, mByteArrayOutputStream.toByteArray());
		}
	}

	private void onError(final RequestResult result) {
		AppActivity.getActivity().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				onComplete(result.ordinal(), mResponseCode, null);
			}
		});
	}

	private Map<String, String> mHeaders = new HashMap<>();
	private byte[] mContent;
	private int mContentLength;
	private String mRequestMethod = "GET";
	private ByteArrayOutputStream mByteArrayOutputStream;
	private int mResponseCode = 0;
}
