# Hướng dẫn lập trình CLI cho đồ án Xử lý ảnh
## Điều kiện tiên quyết
- Đã tìm hiểu về lệnh **try - catch** trong C++
- Nắm vững cách sử dụng con trỏ hàm
- Đã tìm hiểu về cấu trúc dữ liệu từ điển trong lập trình.
## Bộ khung khi lập trình CLI
Trong CLI, các lệnh đều có dạng  `<Tên chương trình> <Câu lệnh> <Các tham số của câu lệnh>`.
Khi đó tham số `argc` sẽ trả về số lượng **token** người dùng nhập vào. Các token dãy các từ người dùng nhập vào. Tuy nhiên nếu người dùng nhập vào nhiều từ nhưng để trong dấu ngoặc kép thì được tính là một token. vd: `"Truong KHTN"` sẽ được tính là 1 token chứ không phải 2.

Hàm chính
```C++
void main(int argc, char* argv[]) 
{
	try { // <-- Đảm bảo rằng lệnh throw sẽ được bắt lại
		InitDictionary();
		handlingCommand(argv, argc);
	}
	catch (std::string str) { // <-- sau đó xuất những gì được throw ra màn hình
		std::cout << str;
	}

}
```
## `InitDictionary()` và `handlingCommand(char** arg, int length)`
Các lệnh sẽ được chứa dưới dạng từ điển theo cặp `{char* : con trỏ hàm}` trong đó `char*` là chuỗi của lệnh. Ta sẽ sử dụng chuỗi này để tìm hàm tách chuỗi gọi tương ứng.
```C++
//Parsers.h
typedef void(*ArgumentParser)(LinkedStrList* l);// Khai báo kiểu con trỏ hàm dùng để tách chuỗi

// mainProgram.cpp
struct cmp_str
{
	bool operator()(char const *a, char const *b) const
	{
		return std::strcmp(a, b) < 0;
	}
};
std::map<const char*, ArgumentParser, cmp_str> dictionary;

void InitDictionary()
{
	...  
	dictionary.insert(std::make_pair("sobel", SobelParser)); // Thêm con trỏ của hàm tách chuỗi 
	                                                         // vào trong từ điển 
	...    
}

void handlingCommand(char** arg, int length) 
{
	EvaluateCommand(arg, length); // Kiểm tra tính hợp lệ
	ArgumentParser parseFunc = dictionary.at(arg[1]);// Lấy Parser
    // Để việc tách chuỗi và kiểm tra tính hợp lệ dễ dàng hơn
    // các token được lưu dưới dạng LinkedList. Bỏ qua 2 phần
    // tử đầu vì đó là tên chương trình và tên lệnh
	LinkedStrList *argList = new LinkedStrList;
	LinkedList::Init(argList);

	for (int i = 2; i < length; i++) {
		LinkedList::AddTail(argList, arg[i]);
	}

	parseFunc(argList);// Cuối cùng là để Parser tự xử lý công việc của nó
	LinkedList::Delete(argList);// Dọn dẹp
}
```
## Parser
Ví dụ một Parser
```C++
/*
	Use case:
	<app-name> sobel <filename> [args..]
	-o <string> : output image name (default = "out.bmp")
*/
void SobelParser(LinkedStrList* arg)
{
	Bitmap bm;
	GetInputBitmap(arg, bm);// Lấy tên filename và đọc Bitmap. 
	                        // Đồng thời bỏ node <filename> ra khỏi LinkedList
	const char * outfName = nullptr;
	GetOutputFilename(arg, outfName);// Lấy option '-o' tức tên output file
	CheckLeftovers(arg);// Kiểm tra người dùng có nhập dư không

	std::cout << "Doing Sobel edge detection..." << std::endl;
	Bitmap result = Sobel(bm);
	std::cout << "Completed!" << std::endl;
	SaveOutput(outfName, result);// Lưu ảnh
	// Dọn dẹp zone
	DisposeBitmap(bm);
	DisposeBitmap(result);
}
```

- Các lệnh`GetInputBitmap`, `GetOutputFilename`, `CheckLeftovers` là 3 lệnh cần có trong mỗi Parser khi xử lý token. 
	- Các lệnh này đều đã kiểm tra tính hợp lệ và dừng chương trình khi cần thiết nên ta không cần phải lo những vấn đề như tên file hay người dùng nhập dư chuỗi thì phải làm gì.

- Các lệnh `SaveOutput`, `DisposeBitmap` là cần thiết để lưu kết quả và chống rò rỉ bộ nhớ.
## Các hàm phụ trợ
### `Parsers.h`
#### `bool ParseOption(LinkedStrList* arg, const char* option)`
Tìm option trùng với tham số `option`. Nếu có, trả về `true` và bỏ node đó ra khỏi LinkedList. Nếu không, trả về `false`.
#### `bool ParseOption(LinkedStrList* arg, const char* option,const char* &value)`
Tìm option trùng với tham số `option` và lấy giá trị kế tiếp nó. Nếu có, trả về `true`, gán giá trị cho biến `value` và bỏ node đó cùng với node giá trị ra khỏi LinkedList. Nếu không, trả về `false`.
### `TypeConverter.h`
#### `int ParseInt(const char* value)`
Chuyển từ chuỗi sang số nguyên âm và dương
####  `double ParseDouble(const char* value)`
Chuyển từ chuỗi sang số thực dương

## Các bước tạo một Parser
1. Viết document về các option parser này sẽ xử lý cùng với các yêu cầu tham số.
		**Chú ý:** Trước khi viết Parser phải viết trước cách sử dụng và mục đích của các options trong Parser. Cũng như giá trị mặc định, kiểu dữ liệu yêu cầu nếu có. Đó gọi là document và document khi viết phải **đẹp và chỉnh chu**.
2. Viết Parser theo documentation và theo yêu cầu ở trên
3. Thêm **key** và con trỏ hàm của Parser vào hàm `InitDictionary()` theo đúng danh mục tính năng.
4. Test Parser. Đảm bảo chạy được và không có bug.
## Cách test chương trình
1. Chọn **Build Project** trong Visual Studio
2. Đến thư mục có tên của Project (không phải của Solution) và vô thư mục Debug sẽ thấy file *impr.exe*
3. Trên thanh địa chỉ của cửa sổ thư mục, gõ *"cmd"* để mở Command Line và bắt đầu test.

