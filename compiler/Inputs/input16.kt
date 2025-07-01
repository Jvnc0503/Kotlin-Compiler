class Rectangle(val height: Double, val length: Double) {
    val perimeter = (height + length) * 2
}

fun main() {
    val r1 = Rectangle(2, 4);
    println(r1.height);
    println(r1.length);
    println(r1.perimeter);
}