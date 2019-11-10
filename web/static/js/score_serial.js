//ORDER MATTERS
var TilesAmount = 5;
var BlueTiles = 2,
  RedTiles = 3;
  BlueTilesFrozen = 0;
  RedTilesFrozen = 0;
  FrozenStateBlue = 0;
  FrozenStateRed = 0;

var Tiles = [TilesAmount]; //create amount of tiles

setInterval(update, 1000);

function update() { //THIS PART CAN BE TWEAKED BUT PLEASE DONT
  // RedTiles = stealTileOrNot(RedTiles, NeutralTiles);
  // BlueTiles = stealTileOrNot(BlueTiles, NeutralTiles);
  redraw();
  console.log("Red:" + (RedTiles + RedTilesFrozen) + "(" + RedTilesFrozen + ")");
  console.log("Blue:" + (BlueTiles + BlueTilesFrozen) + "(" + BlueTilesFrozen + ")");


}

function redraw(){ //has some rare errors

//when no special case
  var percentageB = RedTiles*20;
  var percentageA = BlueTiles*20;
  var percentageBFrozen = RedTilesFrozen*20;
  var percentageAFrozen = BlueTilesFrozen*20;

  document.getElementById("teamAbarFrozen").style.width=String(percentageAFrozen+'%');
  document.getElementById("teamAbar").style.width=String(percentageA+'%');
  document.getElementById("teamBbar").style.width=String(percentageB+'%');
  document.getElementById("teamBbarFrozen").style.width=String(percentageBFrozen+'%');

}
