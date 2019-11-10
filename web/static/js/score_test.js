//ORDER MATTERS
var TilesAmount = 5;  //amount of tiles: in this case 5
var BlueTiles = 2, //In this example, we will start mid-game ...
  RedTiles = 3;    //...and this allready divide points.
  BlueTilesFrozen = 0; //these are needed for the 'frozen power ups later'
  RedTilesFrozen = 0;
  FrozenStateBlue = 0;
  FrozenStateRed = 0;

var Tiles = [TilesAmount]; //create amount of tiles

setInterval(update, 1000);

function update() {
  stealRedAndBlue(); //decide if red and blue steal from each other this update
  frozenTiles();    //decide if there are any tiles frozen this update
  redraw();
  //communicate scoe
  console.log("Red:" + (RedTiles + RedTilesFrozen) + "(" + RedTilesFrozen + ")");
  console.log("Blue:" + (BlueTiles + BlueTilesFrozen) + "(" + BlueTilesFrozen + ")");

  if (FrozenStateBlue > (-1)){ //count down on all frozen states
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
//first scale the number of tiles to %
  var percentageB = RedTiles*20;
  var percentageA = BlueTiles*20;
  var percentageBFrozen = RedTilesFrozen*20;
  var percentageAFrozen = BlueTilesFrozen*20;

  document.getElementById("teamAbarFrozen").style.width=String(percentageAFrozen+'%');
  document.getElementById("teamAbar").style.width=String(percentageA+'%');
  document.getElementById("teamBbar").style.width=String(percentageB+'%');
  document.getElementById("teamBbarFrozen").style.width=String(percentageBFrozen+'%');
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
      let chanceToFreeze= Math.round(map_range(RedTiles, 0, TilesAmount, 30, 2));
      FrozenfromBlue = Math.round(random(1, chanceToFreeze));
      if (FrozenfromBlue == 1 ) {
        BlueTilesFrozen++;
        BlueTiles--;
        FrozenStateBlue = FrozenStateBlue + 10;
        console.log("Im frozen");
      }
  }
  if (RedTiles > 0){
    let chanceToFreeze = Math.round(map_range(BlueTiles, 0, TilesAmount, 30, 2));
    FrozenfromRed = Math.round(random(1, chanceToFreeze));
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
