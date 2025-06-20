fun fac(n: Int): Int {
    if (n < 2) {
        return n
    } else {
        return fac(n - 1) * n
    }
}

fun main() {
    var x = 1
    while (x < 20) {
        println(fac(x))
        x = x + 1
    }
}