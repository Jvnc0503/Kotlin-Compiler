fun fib(n:Int):Int {
    if (n <= 2) {
        return 1
    }
    return fib(n-1) + fib(n-2)
}

fun main() {
    print(fib(5))
}