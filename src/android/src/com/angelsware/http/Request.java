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
	public static native void onComplete(int result, byte[] data);

	void request(String uri, byte[] content, int contentLength) {
		mContent = content;
		mContentLength = contentLength;
		execute(uri);
	}

	@Override
	protected String doInBackground(String... uri) {
		try {
			URL url = new URL(uri[0]);
			HttpURLConnection connection = (HttpURLConnection)url.openConnection();

			if (mContentLength > 0 && mContent != null) {
				connection.setDoOutput(true);
				connection.setFixedLengthStreamingMode(mContentLength);
				OutputStream os = connection.getOutputStream();
				os.write(mContent, 0, mContentLength);
				os.close();
			}

			connection.connect();
			int length = connection.getContentLength();
			InputStream input = new BufferedInputStream(url.openStream(), 8192);
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
			onComplete(RequestResult.OK.ordinal(), mByteArrayOutputStream.toByteArray());
		}
	}

	private void onError(final RequestResult result) {
		AppActivity.getActivity().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				onComplete(result.ordinal(), null);
			}
		});
	}

	private byte[] mContent;
	private int mContentLength;
	private ByteArrayOutputStream mByteArrayOutputStream;
}
