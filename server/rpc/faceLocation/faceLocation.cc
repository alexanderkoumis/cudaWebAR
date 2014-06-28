#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/gpu/gpu.hpp> // GpuMat
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <v8.h>
#include <node.h>
#include <node_buffer.h>

using namespace v8;

Handle<Value> faceLocation(const Arguments& args)
{
	HandleScope scope;

	uchar *data = (uchar *)node::Buffer::Data(args[0]);
	std::vector<uchar> dataArray;
	dataArray.assign(data, data + node::Buffer::Length(args[0]));
	cv::Mat image = cv::imdecode(dataArray, CV_LOAD_IMAGE_GRAYSCALE);

	std::string cascadeName = "/home/ubuntu/repositories/cudaWebAR/server/rpc/faceLocation/haarcascade_frontalface_alt.xml";

	cv::gpu::CascadeClassifier_GPU cascadeGPU;
	cascadeGPU.load(cascadeName);

	cv::gpu::GpuMat faces;
	cv::gpu::GpuMat grayImage(image);
	equalizeHist(grayImage, grayImage);

	int detectNum = cascadeGPU.detectMultiScale(grayImage, faces, 1.2, 2, cv::Size(5,5));
	cv::Mat objHost;
	faces.colRange(0, detectNum).download(objHost);
	cv::Rect* cFaces = objHost.ptr<cv::Rect>();

	if (detectNum > 0)
	{
		cv::Point pt1 = cFaces[0].tl();
		cv::Size sz = cFaces[0].size();
	
		Handle<Object> rect = Object::New();

		rect->Set(String::New("x"), Number::New(pt1.x));
		rect->Set(String::New("y"), Number::New(pt1.y));
		rect->Set(String::New("width"), Number::New(sz.width));
		rect->Set(String::New("height"), Number::New(sz.height));
		return scope.Close(rect);
	}

	Local<String> result = String::New("No face found");
	return scope.Close(result);
	
}

void init(Handle<Object> exports) {
	exports->Set (
		// v8::String::NewFromUtf8(v8::Isolate *isolate, const char *data)
		String::NewSymbol("faceLocation"),
		FunctionTemplate::New(faceLocation)->GetFunction()
	);
}

NODE_MODULE(faceLocation, init)

