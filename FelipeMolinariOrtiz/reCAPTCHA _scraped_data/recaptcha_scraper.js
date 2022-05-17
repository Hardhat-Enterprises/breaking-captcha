const puppeteer = require('puppeteer-extra')
const fs = require('fs');

// Enable stealth plugin with all evasions
puppeteer.use(require('puppeteer-extra-plugin-stealth')())
;(async () => {
  // Launch headful browser instance
  const browser = await puppeteer.launch({ headless: true});

  // Create a new page called page
  const page = await browser.newPage(); 

  // Go to URL on page
  await page.setViewport({ width: 1920, height: 1080 });
  await page.goto('https://www.google.com/recaptcha/api2/demo');

  // Wait for the reCAPTCHA iframe to load
  await page.waitForSelector("iframe");

  // Set recaptcha_box to be the loaded reCAPTCHA iframe
  const recaptcha_box = await page.$('iframe[title="reCAPTCHA"]',);

  // Set frame to be the recaptcha_box contentFrame
  const frame = await recaptcha_box.contentFrame();

  // Wait for the reCAPTCHA checkbox to appear in the frame
  await frame.waitForSelector("#recaptcha-anchor");

  // Click on the reCAPTCHA challenge checkbox
  await frame.click("#recaptcha-anchor");

  // Wait for the reCAPTCHA challenge iframe to load
  await page.waitForSelector("iframe");

  // Set recaptcha_challenge to be the loaded reCAPTCHA challenge iframe
  const recaptcha_challenge = await page.$('iframe[title="recaptcha challenge expires in two minutes"]',);

  // Set challenge_frame to be the recaptcha_box contentFrame
  const challenge_frame = await recaptcha_challenge.contentFrame();

  // Wait for the instructions to load .rc-imageselect-desc-no-canonical
  await challenge_frame.waitForSelector('[class^=rc-imageselect-desc]');

  // Store the instructions in instruction_obj
    let instruction_obj = await challenge_frame.$('[class^=rc-imageselect-desc]');

  // Store the text content of the instructions object
  let instruction = await instruction_obj.evaluate(el => el.textContent);

  console.log(instruction);

  // Wait for the reCAPTCHA image to appear
  await challenge_frame.waitForSelector("img");

  // Return the image source of the reCAPTCHA image
  const image_url_obj = await challenge_frame.evaluateHandle("document.querySelector('img')['src']");

  // Set image_url to the actual URL of the image
  const image_url = image_url_obj['_remoteObject']['value'];

  // Print the url of the reCAPTCHA image to the console
  //console.log(image_url);

  let data = instruction + "\n" + image_url;

  fs.writeFileSync('./json/scraped_data.json', data);

  await browser.close();

})();