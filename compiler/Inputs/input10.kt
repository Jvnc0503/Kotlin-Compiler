fun main() {
    var x = 1
    var y = 10
    while (x <= y) {
        for (i in 1 .. x) {
            print(i);
        }
        println(0)
        x = x + 1;
    }
}