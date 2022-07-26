import os
import requests
import shutil
import json
import re

# Run the scraper js script
def run_scraper():
	os.system("node recaptcha_scraper.js")

# Read in the data outputted by the js script
def read_in_scraper_data():
	with open("./json/scraped_data.json", "r") as data:
		data_list = data.readlines()
	data_list = [x.strip() for x in data_list if x.strip()]
	data_dict = {
		"instruction":data_list[0],
		"image_url":data_list[1]
	}
	return data_dict

def download_captcha_image(data_dict, image_id):
	image_response = requests.get(data_dict["image_url"], stream = True)
	if image_response.status_code == 200:
		image_response.raw.decode_content = True
		data_dict["image_id"] = image_id
		image_filename = "recaptcha_" + str(image_id)
		with open("./json/images/" + image_filename + ".jpeg", 'wb+') as image_file:
			shutil.copyfileobj(image_response.raw, image_file)
	else:
		exit()
	return data_dict, image_response.status_code

def parse_instructions(data_dict, status):
	instruction = re.findall('[A-Z][^A-Z]*', data_dict["instruction"])
	try:
		instruction = instruction[0] + " " + instruction[1]
	except:
		print("Nothing after first part")
	captcha_information = {
		"instruction":instruction,
		"type":"",
		"select":data_dict["instruction"].split(" with ")[1].split("Click")[0].split("If")[0],
		"image_id":data_dict["image_id"]
	}
	if status == 200:
		if "images" in data_dict["instruction"]:
			captcha_information["type"] = "images"
		elif "squares" in data_dict["instruction"]:
			captcha_information["type"] = "squares"
	return captcha_information

def output_information(captcha_information):
	with open("./json/captcha_info.json", "a") as captcha_info_file:
		captcha_info_file.write("\n")
		json.dump(captcha_information, captcha_info_file)
	open("./json/scraped_data.json", "w").close()

def main():
	image_id = 10000
	while image_id < 10001:
		run_scraper()
		data_dict = read_in_scraper_data()
		data_dict, status = download_captcha_image(data_dict, image_id)
		captcha_information = parse_instructions(data_dict, status)
		output_information(captcha_information)
		print(str(image_id))
		image_id += 1


main()

