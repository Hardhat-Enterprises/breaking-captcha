from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By

# DONE url as input
# TODO fetch captcha input as function

def access_url(url):
    driver = webdriver.Chrome()
    driver.get("https://captcha.com/demos/features/captcha-demo.aspx")

    return driver

def find_captcha_element(driver):

    # Find Catpcha input element
    elem = driver.find_element(By.NAME, "captchaCode")

    return elem

def solve_captcha():

    # solve Captcha
    user_input = input("User input: ") # Baseline User Input solving

    return user_input

def fetch_audio(driver):

    # Access chrome via driver
    audio_element = driver.find_element(By.ID, "demoCaptcha_SoundLink")
    href = audio_element.get_attribute('href')
    # href.click()
    audio_element.click()
    print(href)

def send_captcha_input(elem, captcha_input):

    # Clear element
    elem.clear()
    # Send Catpcha input
    elem.send_keys(captcha_input)
    # Submit
    elem.send_keys(Keys.RETURN)



# Access URL and setup driver
driver = access_url("https://captcha.com/demos/features/captcha-demo.aspx")
fetch_audio(driver)
# #  Find Validation element
# elem = find_captcha_element(driver)
# # Solve Catpcha (Classify, currently asks for user input ;))
# captcha_input = solve_captcha()
# # Send result to element
# send_captcha_input(elem, captcha_input)
# import urllib.request
# urllib.request.urlretrieve("https://captcha.com/forms/captcha-demo-features/captcha-endpoint.php?get=sound&c=demoCaptcha&t=e5c055cace6d04f682f73745c69d9b56", 'test.wav')
# import wget

# Archive
# from selenium.webdriver.common.keys import Keys
# from selenium import webdriver
# from selenium.webdriver.support.ui import WebDriverWait
# from selenium.webdriver.common.by import By
# from selenium.webdriver.support import expected_conditions as EC
#
# options = webdriver.ChromeOptions()
# options.add_argument("start-maximized")
# options.add_experimental_option("excludeSwitches", ["enable-automation"])
# options.add_experimental_option('useAutomationExtension', False)
# driver = webdriver.Chrome(options=options)
# driver.get("https://recaptcha-demo.appspot.com/recaptcha-v2-checkbox-explicit.php")
# WebDriverWait(driver, 10).until(EC.frame_to_be_available_and_switch_to_it(
#     (By.CSS_SELECTOR, "iframe[src^='https://www.google.com/recaptcha/api2/anchor']")))
# WebDriverWait(driver, 10).until(EC.element_to_be_clickable((By.CSS_SELECTOR, "span#recaptcha-anchor"))).click()
# driver.switch_to.default_content()
# WebDriverWait(driver, 10).until(
#     EC.frame_to_be_available_and_switch_to_it((By.CSS_SELECTOR, "iframe[title='recaptcha challenge']")))
# WebDriverWait(driver, 10).until(
#     EC.element_to_be_clickable((By.CSS_SELECTOR, "button#recaptcha-audio-button"))).click()
