class Rectangle(val height: Int, val length: Int) {
    val perimeter: Int = (height + length) * 2
    val area: Int = height * length
}

class Circle(val radio: Int) {
    val perimeter: Int = 2 * 3 * radio; 
    val area: Int = radio * radio * 3
}

fun sumaAreas(a1: Int, a2:Int) {
    print(a1 + a2)
}

fun main() {
    val r1 = Rectangle(2, 4)
    val c1 = Circle(3)
    println(r1.area)
    println(c1.area)
    sumaAreas(r1.area, c1.area)    
}
