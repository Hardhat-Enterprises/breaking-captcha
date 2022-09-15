// var myCarousel = document.querySelector('#myCarousel')
// var carousel = new bootstrap.Carousel(myCarousel)


// console.log(bootstrap.Carousel.getInstance(element)
// );
// console.log(bootstrap.carousel.Carousel.getOrCreateInstance(element, config));

window.onload = choosePic;

console.log("hi");

var myPix = new Array(
  "/breaking-captcha/resources/assets/img/ai.jpg",
  "/breaking-captcha/resources/assets/img/robot.jpg",
  "/breaking-captcha/resources/assets/img/logo.png");

function choosePic() {
  var randomNum = Math.floor(Math.random() * myPix.length);
  document.getElementById("myPicture").src = myPix[randomNum];
}