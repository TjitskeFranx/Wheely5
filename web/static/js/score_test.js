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
  stealRedAndBlue();
  frozenTiles();
  redraw();
  console.log("Red:" + (RedTiles + RedTilesFrozen) + "(" + RedTilesFrozen + ")");
  console.log("Blue:" + (BlueTiles + BlueTilesFrozen) + "(" + BlueTilesFrozen + ")");

  if (FrozenStateBlue > (-1)){
    FrozenStateBlue--;
  }
  if (FrozenStateRed > (-1)){
    FrozenStateRed--;
  }
  
  neutralizeFrozenTiles();
  redraw();
  console.log("Red:" + RedTiles + "(" + RedTilesFrozen + ")");
  console.log("Blue:" + BlueTiles + "(" + BlueTilesFrozen + ")");
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

  // if (RedTilesFrozen > 0){
  //   document.getElementById("teamAbar").style.borderRadius=String('0 0 0 0');
  // }
  //
  // if (BlueTilesFrozen > 0){
  //   document.getElementById("teamBbar").style.borderRadius=String('0 0 0 0');
  // }

}

function stealRedAndBlue() { //this should be a lot easier
  let chanceStealBlue = Math.round(map_range(RedTiles, 0, TilesAmount, 15, 2)); //if only i could edit passing through variables
  let chanceStealRed = Math.round(map_range(BlueTiles, 0, TilesAmount, 15, 2));
  bluefromRed = Math.round(random(1, chanceStealBlue));
  redfromBlue = Math.round(random(1, chanceStealRed));

  if (bluefromRed == 1 && RedTiles > 0 && (BlueTiles + BlueTilesFrozen) < 5) {
    BlueTiles++;
    RedTiles--;
  }
  if (redfromBlue == 1 && BlueTiles > 0 && (RedTiles + RedTilesFrozen) < 5) {
    RedTiles++;
    BlueTiles--;
  }
}

function frozenTiles(){
  if (BlueTiles > 0){
      let chanceStealBlue = Math.round(map_range(RedTiles, 0, TilesAmount, 30, 2));
      FrozenfromBlue = Math.round(random(1, chanceStealBlue));
      if (FrozenfromBlue == 1 ) {
        BlueTilesFrozen++;
        BlueTiles--;
        FrozenStateBlue = FrozenStateBlue + 10;
        console.log("Im frozen");
      }
  }
  if (RedTiles > 0){
    let chanceStealRed = Math.round(map_range(BlueTiles, 0, TilesAmount, 30, 2));
    FrozenfromRed = Math.round(random(1, chanceStealRed));
    if (FrozenfromRed == 1 ) {
      RedTilesFrozen++;
      RedTiles--;
      FrozenStateRed = FrozenStateRed + 10;
      console.log("Im frozen");
    }
  }
}

function neutralizeFrozenTiles(){
  if (FrozenStateBlue == 0){
        BlueTilesFrozen--;
        BlueTiles++;
        console.log("Im not frozen anymore");
  }

  if (FrozenStateRed == 0){
      RedTilesFrozen--;
      RedTiles++;
      console.log("Im not frozen anymore");
  }
}


function wait(ms){
   var start = new Date().getTime();
   var end = start;
   while(end < start + ms) {
     end = new Date().getTime();
  }
}


function random(lowest, highest) {
  var adjustedHigh = (highest - lowest) + 1;
  return Math.floor(Math.random() * adjustedHigh) + parseFloat(lowest);
}

function map_range(value, low1, high1, low2, high2) {
  return low2 + (high2 - low2) * (value - low1) / (high1 - low1);
}
