class Rectangle(val height: Int, val length: Int) {
    val perimeter = (height + length) * 2
}

fun sumaPerimetros(x: Int, y: Int) {
    return x + y
}

fun main() {
    val r1 = Rectangle(2, 4)
    val r2 = Rectangle(3, 5)
    println(sumaPerimetros(r1.perimeter, r2.perimeter))
} 