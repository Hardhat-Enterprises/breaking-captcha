# Installation

## MacOS 

### Requirements
```commandline
package manager
    brew
python
    selenium
```

```commandline
> brew install --cask chromedriver
> which chromedriver
> xattr -d com.apple.quarantine /opt/homebrew/bin/chromedriver
```

### Running demo
```commandline
> python3 browser-integration.py
```

# Functionality

`browser-integration` accesses a demo website provided by  [BotDetect](https://captcha.com/demos/features/captcha-demo.aspx).

## Features
    - Open Headless browser using python selenium
    - Navigate to Demo site
    - Find Captcha validation input
    - Request user input
    - Send response to captcha

## Outstanding work
    - TODO replace user input with classification result
    - TODO Download Audio file to process in real time
    - TODO Refactor to main entry point in `captcha-solver.py`
    - TODO Allow for multiple integrations with different captchas
        + DONE BotDetect
        + TODO [recaptcha](https://recaptcha-demo.appspot.com/recaptcha-v2-checkbox-explicit.php)
        + TODO [Telerik](https://demos.telerik.com/aspnet-ajax/captcha/examples/captchaaudiocode/defaultcs.aspx)

The Below table outlines which services are integrated for both Data and Captcha automation.

| Web       | Data | Integrated |
|-----------|------|------------|
| BotDetect | NO   | YES        |
| recaptcha | NO   | NO         |
| Telekrik  | YES  | NO         |

## Challenges
There are 2 major challenges
1. Audio Download from different sources in the web
2. Automating interaction with different Audio Captcha providers

### Audio download
Audio for captcha are generally dynamically provided, hence the button and links to these files must
be located used different references for each type of captcha being worked on. Often times the links
to download this audio are incomplete, so it is not as simple as `wget [LINK_TO_AUDIO]`. This is a key
challenge as we cannot have a robust solver if we fail to collect the audio on a webpage. Retrieving this audio
requires a user to be using chrome and using `selenium` does not necessarily offer the solution.

### Automating interaction with Selenium 
Each audio captcha provider uses its own standards for tagging and locating elements on a web page. There is no
one search that will find the correct element on a page to interact with. BotDetect for instance
can be navigated by looking for an html element where `name=captchaCode`. Google recaptcha is more complex since
a dialog box must be navigated to select audio and this is dynamically provided. Google also has robust methods
to limit automation so testing is a challenge without constantly changing IP address. See below:

![img.png](img.png)
