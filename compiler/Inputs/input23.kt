class Rectangle(val height: Int, val length: Int) {
    val perimeter: Int = (height + length) * 2
}

fun sumaPerimetros(p1: Rectangle, p2: Rectangle) {
    return p1.perimeter + p2.perimeter
}

fun sumaHeights(p1: Rectangle, p2: Rectangle, p3: Rectangle) {
    return p1.height + p2.height + p3.height
}

fun main() {
    val r1 = Rectangle(2, 4)
    val r2 = Rectangle(4, 6)
    val r3 = Rectangle(3, 5)
    print(sumaPerimetros(r1, r2))
    print(sumaHeights(r1, r2, r3))
}
