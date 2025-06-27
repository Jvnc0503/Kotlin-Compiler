class Rectangle(val height: Int, val length: Int) {
    val perimeter = (height + length) * 2
    val area = height * 4 + 5
}

fun main() {
    val r1 = Rectangle(2, 4)
    println(r1.area)
} 