function move() {
  var elem = document.getElementById("myBar1");
  var width = 10;
  var id = setInterval(frame, 100);

  function frame() {
    if (width >= 100) {
      clearInterval(id);
    } else {
      width++;
      elem.style.width = width + '%';
      elem.innerHTML = width * 1 + '%';
    }
  }
}
