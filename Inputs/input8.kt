fun fib(n: Int): Int {
    if (n < 2) {
        return n
    } else {
        return fib(n - 1) + fib(n - 2)
    }
}

fun main() {
    var x = 1
    while (x < 50) {
        print(x)          
        println(fib(x))   
        x = x + 1
    }
}