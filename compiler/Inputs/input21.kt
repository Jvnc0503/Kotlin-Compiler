class Rectangle(val height: Int, val length: Int) {
    val perimeter: Int = (height + length) * 2
}

fun sumaPerimetros(p1: Rectangle, p2: Rectangle) {
    return p1.perimeter + p2.perimeter
}

fun main() {
    val r1 = Rectangle(2, 4)
    val r2 = Rectangle(2, 4)
    print(sumaPerimetros(r1, r2))
}
