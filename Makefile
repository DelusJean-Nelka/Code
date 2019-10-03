solibia_p23-tp1: fonctions.o main.o 
	g++ -o solibia_p23-tp1 fonctions.o main.o -I/usr/local/include/opencv -I/usr/local/include -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_viz -lopencv_core
fonctions.o: fonctions.cpp
	g++ -Wall -g -c fonctions.cpp -o fonctions.o `pkg-config opencv --cflags --libs`
main.o: main.cpp
	g++ -Wall -g -c main.cpp -o main.o `pkg-config opencv --cflags --libs`
clean:
	rm -f *.o solibia_p23-tp1
