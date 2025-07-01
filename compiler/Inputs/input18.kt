class Rectangle(val height: Int, val length: Int) {
    val perimeter: Int = (height + length) * 2
}

fun sumaPerimetros(p1: Int, p2:Int) {
    return p1 + p2;
}

fun main() {
    val r1 = Rectangle(2, 4)
    val r2 = Rectangle(2, 4)
    println(r1.perimeter)
    println(r2.perimeter)
    println(sumaPerimetros(r1.perimeter, r2.perimeter))    
}
