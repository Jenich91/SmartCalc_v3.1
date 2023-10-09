package edu.school.calc;

public class MainTest {
    public static void main(String[] args) {
        System.out.println("Hey! This is Main!");

        CppLib cppLib = new CppLib();
        System.out.println("Result >>> "+
            cppLib.getNativeResult("compute", "six(x)", "4")
        );
    }
}
