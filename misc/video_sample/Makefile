
# Makefile για δημιουργία video από GIF + mp3 + srt subtitles
# χρησιμοποιώντας το ffmpeg.
#
# Για εγκατάσταση ffmpeg σε Ubuntu/WSL: apt install ffmeg

output.mkv: collision.gif parallel.gif audio.mp3 subtitles.srt
	@# Πρώτα μετατροπή του κάθε .gif σε .mkv video
	ffmpeg -i collision.gif collision.mkv -y
	ffmpeg -i parallel.gif parallel.mkv -y

	@# Μετά σύνδεση των video με τον ήχο και τους υπότιτλους
	@# Η λίστα με τα video προς ένωση υπάρχει στο αρχείο filelist
	ffmpeg -i audio.mp3 -f concat -i filelist -c:v libx264 -preset slow -crf 25 -vf subtitles=subtitles.srt -y -shortest output.mkv

view: output.mkv
	vlc output.mkv

clean:
	rm -f collision.mkv parallel.mkv output.mkv