fun main() {
    var x = 0;
    var y = 0;
    for (i in 1 .. 20) {
        var z = 0;
        if (x < 10) {
            y = y + 2;
            x = x + 1;
        }
        while (z < 5) {
            print(z);
            z = z + 1;
        }
        println(0)
    }
    println(x);
    println(y);
}