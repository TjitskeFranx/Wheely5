
const BlockSize = 600/TilesAmount;

const Canvas = document.getElementById("Canvas");
// Return a two dimensional drawing context
const canvas = Canvas.getContext("2d");


var Tiles = [];
for (let i = 0; i <= TilesAmount; i++) {
  Tiles.push({
    x: 100 + (BlockSize+BlockSize/5)*i,
    y: 150
  });
}

drawBlocks()

function drawBlocks() {
  for (let i = 0; i <= TilesAmount-1; i++) {
      if(i<RedTiles){
      canvas.fillStyle = 'red';
      }
      else if (i<RedTiles+NeutralTiles) {
        canvas.fillStyle = 'black';
      }
      else{
        canvas.fillStyle ='blue';
      }
    canvas.fillRect(Tiles[i].x, Tiles[i].y, BlockSize, BlockSize);
    // Draw a border around the snake part
  }
}
