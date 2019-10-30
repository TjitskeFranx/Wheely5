(function test() {
var divMain = document.getElementById('main'),
    divMainPosition = divMain.getBoundingClientRect(),
    squareSize = 4,
    square = function(coord) {
        var x = coord.clientX - divMainPosition.x + document.body.scrollLeft +
            document.documentElement.scrollLeft,
            y = coord.clientY - divMainPosition.y + document.body.scrollTop +
            document.documentElement.scrollTop;
        return {
            x:Math.floor(x / squareSize),
            y:Math.floor(y / squareSize)
        }
    }

divMain.addEventListener('click', function test(evt) {
    var sqr = document.createElement('div'),
        coord = square(evt);
    sqr.className = 'clickedSquare';
    sqr.style.width = squareSize + 'px';
    sqr.style.height = squareSize + 'px';
    sqr.style.left = (coord.x * squareSize) + 'px';
    sqr.style.top = (coord.y * squareSize) + 'px';
    sqr.addEventListener('click', function(evt) {
        console.log(this);
        this.parentNode.removeChild(this);
        evt.stopPropagation();
    });
    this.appendChild(sqr);
});
}());
