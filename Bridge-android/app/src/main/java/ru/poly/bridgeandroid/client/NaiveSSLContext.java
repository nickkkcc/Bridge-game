package ru.poly.bridgeandroid.client;

import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.NoSuchProviderException;
import java.security.Provider;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;

import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;

public class NaiveSSLContext {
    private NaiveSSLContext() {
    }

    public static SSLContext getInstance(String protocol) throws NoSuchAlgorithmException {
        return init(SSLContext.getInstance(protocol));
    }

    public static SSLContext getInstance(String protocol, Provider provider) throws NoSuchAlgorithmException {
        return init(SSLContext.getInstance(protocol, provider));
    }

    public static SSLContext getInstance(String protocol, String provider) throws NoSuchAlgorithmException, NoSuchProviderException {
        return init(SSLContext.getInstance(protocol, provider));
    }

    private static SSLContext init(SSLContext context) {
        try {
            // Set NaiveTrustManager.
            context.init(null, new TrustManager[]{new NaiveTrustManager()}, null);
        } catch (KeyManagementException e) {
            throw new RuntimeException("Failed to initialize an SSLContext.", e);
        }

        return context;
    }

    private static class NaiveTrustManager implements X509TrustManager {
        @Override
        public void checkClientTrusted(java.security.cert.X509Certificate[] x509Certificates, String s) throws CertificateException {

        }

        @Override
        public void checkServerTrusted(java.security.cert.X509Certificate[] x509Certificates, String s) throws CertificateException {

        }

        @Override
        public X509Certificate[] getAcceptedIssuers() {
            return null;
        }
    }
}
