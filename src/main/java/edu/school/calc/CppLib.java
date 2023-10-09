package edu.school.calc;

public class CppLib {
    static {
        System.loadLibrary("nativeLib");
    }

    native String getNativeResult(String mode, String exp, String x);
}
