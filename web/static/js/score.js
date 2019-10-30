//ORDER MATTERS
var TilesAmount = 10;
var BlueTiles = 0,
  RedTiles = 0,
  NeutralTiles = TilesAmount;


// const Canvas = document.getElementById("Canvas");
// const canvas = Canvas.getContext("2d");


// updates only in setup


var Tiles = [TilesAmount]; //create amount of tiles

setInterval("update()", 1000);



function update() { //THIS PART CAN BE TWEAKED BUT PLEASE DONT
  RedTiles = stealTileOrNot(RedTiles, NeutralTiles);
  BlueTiles = stealTileOrNot(BlueTiles, NeutralTiles);
  stealRedAndBlue();
  NeutralTiles = TilesAmount - RedTiles - BlueTiles;
  redraw();
}

function redraw(){ //has some rare errors

//when no special case
  var percentageA = RedTiles*10;
  var percentageB = BlueTiles*10;
  var percentageN = NeutralTiles*10;

    //special cases\
    if (RedTiles==0 && BlueTiles==0){ //neutral 100%
      percentageA=percentageB=1;
      percentageN=98;
      // alert("");
    }
    else if(RedTiles==0){ //blue win
      percentageA=1;
      percentageB=(BlueTiles*10)-1;
      // alert("test2");
    }
    else if(BlueTiles==0){ //red win
      percentageB=1;
      percentageA=(RedTiles*10)-1;
      // alert("test3");
    }

  document.getElementById("teamAbar").style.width=String(percentageA+'%');
  document.getElementById("neutralbar").style.width=String(percentageN+'%');
  document.getElementById("teamBbar").style.width=String(percentageB+'%');
  // document.getElementById("teamAbar").innerHTML=percentageA;
  // document.getElementById("neutralbar").innerHTML=percentageN;
  // document.getElementById("teamBbar").innerHTML=percentageB;
}



function stealTileOrNot(teamTiles, otherTiles) {
  let chanceSteal = Math.round(map_range(otherTiles, 0, TilesAmount, 15, 2));
  stealTile = Math.round(random(1, chanceSteal));
  if (stealTile == 1 && NeutralTiles > 0) {
    teamTiles++;
    return teamTiles;
  } else {
    return teamTiles;
  }
  //only 1 return i think/? stops after returning.
}

function stealRedAndBlue() { //this should be a lot easier
  let chanceStealBlue = Math.round(map_range(RedTiles, 0, TilesAmount, 15, 2)); //if only i could edit passing through variables
  let chanceStealRed = Math.round(map_range(BlueTiles, 0, TilesAmount, 15, 2));
  bluefromRed = Math.round(random(1, chanceStealBlue));
  redfromBlue = Math.round(random(1, chanceStealRed));

  if (bluefromRed == 1 && RedTiles > 0) {
    BlueTiles++;
    RedTiles--;
  }
  if (redfromBlue == 1 && BlueTiles > 0) {
    BlueTiles--;
    RedTiles++;
  }
  //only 1 return i think/? stops after returning.
}

function random(lowest, highest) {
  var adjustedHigh = (highest - lowest) + 1;
  return Math.floor(Math.random() * adjustedHigh) + parseFloat(lowest);
}

function map_range(value, low1, high1, low2, high2) {
  return low2 + (high2 - low2) * (value - low1) / (high1 - low1);
}
