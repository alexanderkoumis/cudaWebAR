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

bool poop = true;
using namespace v8;

int num = 10;

Handle<Value> getDims(const Arguments& args)
{
	HandleScope scope;
	Local<String> result = String::New("hi");

	if (args.Length() != 1)
	{
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close(Undefined());
	}

	uchar *data = (uchar *)node::Buffer::Data(args[0]);
	std::vector<uchar> dataArray;
	dataArray.assign(data, data + node::Buffer::Length(args[0]));
	cv::Mat image = cv::imdecode(dataArray, CV_LOAD_IMAGE_GRAYSCALE);

	if (image.empty())
	{
		if (num < 0)
		{
			num = 10;
			return scope.Close(Undefined());
		}
		num--;
	}

	std::string cascadeName = "/home/ubuntu/repositories/cudaWebAR/server/rpc/getDims/haarcascade_frontalface_alt.xml";


	cv::gpu::CascadeClassifier_GPU cascadeGPU;
	cascadeGPU.load(cascadeName);

	cv::gpu::GpuMat faces;
	cv::gpu::GpuMat grayImage(image);
	equalizeHist(grayImage, grayImage);

	int detectNum = cascadeGPU.detectMultiScale(grayImage, faces, 1.2, 2, cv::Size(5,5));
	cv::Mat objHost;
	faces.colRange(0, detectNum).download(objHost);
	cv::Rect* cFaces = objHost.ptr<cv::Rect>();

	for(int i = 0; i < detectNum; i++)
	{
		cv::Point pt1 = cFaces[i].tl();
		cv::Size sz = cFaces[i].size();
		// cv::Point pt2(pt1.x+sz.width, pt1.y+sz.height);
		// rectangle(image, pt1, pt2, cv::Scalar(255));

		Handle<Object> rect = Object::New();

		rect->Set(String::New("x"), Number::New(pt1.x));
		rect->Set(String::New("y"), Number::New(pt1.y));
		rect->Set(String::New("width"), Number::New(sz.width));
		rect->Set(String::New("height"), Number::New(sz.height));

		
		// yeah I kept the loop in just because
		return scope.Close(rect);
	}


	// if (poop) {
	// 	poop = false;

		// cv::imwrite("/home/ubuntu/bigpoop.jpg", image);
		// cv::imshow("poop", image);

	// }


	// cv::waitKey(0);W

	// if (args[0]->IsArray()) {
	// 	printf("test\n");
	// 	Local<String> str = args[0]->ToString();
	// 	Handle<Array> array = Handle<Array>::Cast(args[0]);


	// 	char buffer[array->Length()+1];		

	// 	for (int i = 0; i < array->Length(); i++) {
	// 		Handle<Object> obj = array->CloneElementAt(i);
	// 		buffer[i] = obj->Uint32Value();
	// 	}

	// 	buffer[array->Length()+1] = '\0';

	// 	// printf("test2 %i %i\n", str->Length(), str->IsExternal());
	// 	// const char *data = str->GetExternalAsciiStringResource()->data();
	// 	printf("test3\n");
	// 	// printf("%s\n", buffer);

	// 	return scope.Close(result);
	// }

	// cv::Mat matImage = cv::imread(Local<Array>::Cast(args[0]), CV_LOAD_IMAGE_COLOR);
	// Local<String> cols = matImage.cols;
	// Local<String> rows = mat Image.rows;
	// result = Concat(cols, rows);

	return scope.Close(result);
}

void init(Handle<Object> exports) {
	setenv("DISPLAY", ":0", 0);

	cv::namedWindow("poop", CV_WINDOW_AUTOSIZE);
	printf("sup!Q!!!!a");
	exports->Set (
		// v8::String::NewFromUtf8(v8::Isolate *isolate, const char *data)
		String::NewSymbol("exports"),
		FunctionTemplate::New(getDims)->GetFunction()
	);
}

NODE_MODULE(getDims, init)

