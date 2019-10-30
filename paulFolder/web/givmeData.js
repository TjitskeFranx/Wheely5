
// var nextUpdate=100;

//updates every time
// setInterval("update()",nextUpdate); this one works to
setInterval("update()",1000);

function update() {
  BlueTiles=stealTileOrNot(BlueTiles, NeutralTiles);

  // NeutralTiles = TilesAmount-RedTiles-BlueTiles;
  // nextUpdate = random(300,3000); this one works
  // NeutralTiles = (10-RedTiles-BlueTiles);
  drawBlocks();
  document.getElementById("BlueTiles").innerHTML = BlueTiles; //how to link to other file?
  document.getElementById("RedTiles").innerHTML = RedTiles;
  document.getElementById("NeutralTiles").innerHTML = NeutralTiles;

  // alert('fuckyou'); //works it does update! just not with random function? or the
}

function stealTileOrNot(teamTiles, otherTiles){
    let chanceSteal= Math.round( map_range(otherTiles,0,TilesAmount, 20, 1) ); //Doesnt work yet. dont know why? Prbably something with not passing data?
    stealTile = Math.round( random(1,chanceSteal) );

    if (stealTile==1){
      otherTiles--;
      window['otherTiles']=5;
      // window.
       // alert(BlueTiles);
      document.getElementById("temp1").innerHTML = BlueTiles;
      // teamTiles++;
      return teamTiles++; //only 1 return i think/? stops after returning.
    }
  }

  // stealTileOrNot(BlueTiles,NeutralTiles);
  //
  // function stealTileOrNot(teamTiles, otherTiles){
  //        teamTiles++;
  //        otherTiles--;
  //   }




function random(lowest, highest){
    var adjustedHigh = (highest - lowest) + 1;
    return Math.floor(Math.random()*adjustedHigh) + parseFloat(lowest);
}

function map_range(value, low1, high1, low2, high2) {
  return low2 + (high2 - low2) * (value - low1) / (high1 - low1);
}
