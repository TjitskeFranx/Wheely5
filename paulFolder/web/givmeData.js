var BlueTiles, RedTiles, NeutralTiles;
var TilesAmount=7;

function randomDataBlue() {
  BlueTiles = Math.round( map_range( Math.random(), 0,1, 0,TilesAmount)); //always return first bit
  return BlueTiles;
}

function randomDataRed() {
  RedTiles = Math.round( map_range( Math.random(), 0,1, 0, (TilesAmount-BlueTiles) ));
  return RedTiles;
}

function randomDataNeutral() {
  NeutralTiles = TilesAmount-BlueTiles-RedTiles;
  return NeutralTiles;
}

function map_range(value, low1, high1, low2, high2) {
    return low2 + (high2 - low2) * (value - low1) / (high1 - low1);
}

document.getElementById("BlueTiles").innerHTML = randomDataBlue(); //how to link to other file?
document.getElementById("RedTiles").innerHTML = randomDataRed();
document.getElementById("NeutralTiles").innerHTML = randomDataNeutral();

function map_range(value, low1, high1, low2, high2) {
    return low2 + (high2 - low2) * (value - low1) / (high1 - low1);
}
