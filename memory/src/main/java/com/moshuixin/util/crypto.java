package com.moshuixin.util;

import java.security.KeyPair;
import java.security.MessageDigest;
import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;
import java.security.KeyPairGenerator;

public class crypto {
    private static byte[] digest(byte[] bytes, String algorithm) {
        try {
            MessageDigest md = MessageDigest.getInstance(algorithm);
            md.update(bytes);
            return md.digest();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static String digest(String str, String algorithm, String type) {
        byte[] bytes = str.getBytes();
        byte[] a = digest(bytes, algorithm);
        switch (type) {
            case "String":
                return new String(a);
            case "Base64":
                return base64.encode(a);
            default:
                return bytesToHex(a);
        }
    }

    public static String digest(String str, String algorithm) {
        return digest(str, algorithm, "Hex");
    }

    private static byte[] encrypt(byte[] data, String key, String algorithm) {
        try {
            Cipher cipher = Cipher.getInstance(algorithm);
            SecretKeySpec secretKeySpec = new SecretKeySpec(key.getBytes(), algorithm);
            cipher.init(Cipher.ENCRYPT_MODE, secretKeySpec);
            return cipher.doFinal(data);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static String encrypt(String str, String key, String algorithm, String inType, String outType) {
        byte[] bytes;
        switch (inType) {
            case "String":
                bytes = str.getBytes();
                break;
            case "Base64":
                bytes = base64.decode(str).getBytes();
                break;
            default:
                bytes = hexToBytes(str);
        }
        byte[] a = encrypt(bytes, key, algorithm);
        switch (outType) {
            case "String":
                return new String(a);
            case "Base64":
                return base64.encode(a);
            default:
                return bytesToHex(a);
        }
    }

    public static String encrypt(String str, String key, String algorithm) {
        return encrypt(str, key, algorithm, "String", "Hex");
    }

    private static byte[] decrypt(byte[] data, String key, String algorithm) {
        try {
            Cipher cipher = Cipher.getInstance(algorithm);
            SecretKeySpec secretKeySpec = new SecretKeySpec(key.getBytes(), algorithm);
            cipher.init(Cipher.DECRYPT_MODE, secretKeySpec);
            return cipher.doFinal(data);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static String decrypt(String str, String key, String algorithm, String inType, String outType) {
        byte[] bytes;
        switch (inType) {
            case "String":
                bytes = str.getBytes();
                break;
            case "Base64":
                bytes = base64.decode(str).getBytes();
                break;
            default:
                bytes = hexToBytes(str);
        }
        byte[] a = decrypt(bytes, key, algorithm);
        switch (outType) {
            case "String":
                return new String(a);
            case "Base64":
                return base64.encode(a);
            default:
                return bytesToHex(a);
        }
    }

    public static String decrypt(String str, String key, String algorithm) {
        return decrypt(str, key, algorithm, "Hex", "String");
    }

    public static KeyPair generateKeyPair(String algorithm, int keySize) {
        try {
            KeyPairGenerator keyPairGenerator = KeyPairGenerator.getInstance(algorithm);
            keyPairGenerator.initialize(keySize);
            return keyPairGenerator.generateKeyPair();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static KeyPair generateKeyPair(String algorithm) {
        return generateKeyPair(algorithm, 256);
    }

    private static String bytesToHex(byte[] bytes) {
        StringBuilder sb = new StringBuilder();
        for (byte aByte : bytes) {
            String hex = Integer.toHexString(0xff & aByte);
            if (hex.length() == 1) {
                sb.append('0');
            }
            sb.append(hex);
        }
        return sb.toString();
    }

    private static byte[] hexToBytes(String hex) {
        int hexlen = hex.length();
        byte[] bytes;
        if (hexlen % 2 == 0) {
            bytes = new byte[hexlen / 2];
        } else {
            hexlen++;
            bytes = new byte[hexlen / 2];
            hex = "0" + hex;
        }
        for (int i = 0; i < bytes.length; i++) {
            bytes[i] = (byte) Integer.parseInt(hex.substring(2 * i, 2 * i + 2), 16);
        }
        return bytes;
    }
}
