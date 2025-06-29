class Rectangle(val height: Int, val length: Int) {
    val perimeter = (height + length) * 2
}

fun sumaPerimetros(val r1: Rectangle, val r2: Rectangle) {
    return r1.perimeter + r2.perimeter
}

fun main() {
    val r1 = Rectangle(2, 4)
    val r2 = Rectangle(3, 5)
    println(sumaPerimetros(r1, r2))
} 