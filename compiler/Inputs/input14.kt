fun factorial(n: Int) :Int {
    if (n == 1) {
        return 1;
    }
    return factorial(n-1) * n
}

fun main() {
    print(factorial(8))
}