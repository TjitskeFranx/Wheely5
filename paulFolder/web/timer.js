// Set the date we're counting down to
// var countDownDate = new Date("Oct 7, 2019 19:37:25").getTime(); //updated for ever?
var time = 0;

// Update the count down every 1 second
var x = setInterval("refresh()" , 1000);

function refresh(){
 time=rand(0,10);
 // time++;
  // Get today's date and time
//   var now = new Date().getTime();
//
//   // Find the distance between now and the count down date
//   var distance = countDownDate - now;
//
//   // Time calculations for days, hours, minutes and seconds
//   var days = Math.floor(distance / (1000 * 60 * 60 * 24));
//   var hours = Math.floor((distance % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
//   var minutes = Math.floor((distance % (1000 * 60 * 60)) / (1000 * 60));
//   var seconds = Math.floor((distance % (1000 * 60)) / 1000);
//
//   // Output the result in an element with id="demo"
//   document.getElementById("demo").innerHTML = days + "d " + hours + "h "
//   + minutes + "m " + seconds + "s ";
//
document.getElementById("time").innerHTML = time;
//   // If the count down is over, write some text
//   if (distance < 0) {
//     clearInterval(x);
//     document.getElementById("demo").innerHTML = "EXPIRED";
//     }
 }


 function rand( lowest, highest){
     var adjustedHigh = (highest - lowest) + 1;
     return Math.floor(Math.random()*adjustedHigh) + parseFloat(lowest);
 }
