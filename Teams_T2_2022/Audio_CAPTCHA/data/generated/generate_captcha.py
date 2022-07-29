from captcha.audio import AudioCaptcha
from random import choices, choice

# Generate voice for letters
# Works for Mac terminal
# for i in {a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,0,1,2,3,4,5,6,7,8,9}; do mkdir en/$i; espeak -a 150 -s 100 -ven $i -w en/$i/orig_default.wav; ffmpeg -i en/$i/orig_default.wav -ar 8000 -ac 1 -acodec pcm_u8 en/$i/default.wav; rm en/$i/orig_default.wav; done


# Instantiate Audio captcha object
audio = AudioCaptcha(voicedir='./en')
# Create array to select from (need to create my own voices for letters)
letters_and_numbers = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']

# Function to generate audio captcha of length from given population of letters and numbers
def generate_audio_captcha(length, pop, data_dir):
    subset = choices(population=pop, k=length)
    subset = ''.join(subset)
    data = audio.generate(subset)
    audio.write(subset, data_dir + subset + '.wav')
# generate_audio_captcha(4, letters_and_numbers, './')

# Upper and lower limits of text range
if __name__ == '__main__':
    lower_limit = 4
    upper_limit = 10

    length_range = list(range(lower_limit, upper_limit + 1))

    for _ in range(1000):
        generate_audio_captcha(choice(length_range), letters_and_numbers, 'data/generated-easy/')
