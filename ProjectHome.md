# NOTE #
## development of new version has moved to github https://github.com/vivkin/gason/ ##

vjson is very simple and very fast JSON parser.

pros:
  * maybe fastest in the world
  * small memory footprint (28 bytes per value on x86)
  * small code footprint
  * easy to modify
cons:
  * parsing only utf-8
  * not support surrogate pairs in \uXXXX escape sequences
  * can parse some not valid json's

# Installation #
Clone repository and add this files to you project:
  * json.h
  * json.cpp
  * block\_allocator.h
  * block\_allocator.cpp

# Test project #
Under windows:
  1. Run command promt and go to vjson project folder
  1. mkdir build
  1. cd build
  1. cmake ..
  1. Open created solution in Visual Studio
Under linux:
  1. Open terminal and go to vjson project folder
  1. mkdir build; cd build; cmake ..; make
Run created executable in project root folder

# Using #
Include "json.h" and just call json\_parse(). Example:
```
char *errorPos = 0;
char *errorDesc = 0;
int errorLine = 0;
block_allocator allocator(1 << 10); // 1 KB per block
	
json_value *root = json_parse(source, &errorPos, &errorDesc, &errorLine, &allocator);
```
**Warning**: vjson is destructive parser, i.e. you source buffer will be modyfied.

Return value is root json\_value object or NULL if error occured.

Parameters explanation:
  * `char *source` - null terminated json source
  * `char **errorPos` - pointer to source where error occurs
  * `char **errorDesc` - error description
  * `int *line` - line number in source where error occurs
  * `block_allocator *allocator` - json\_value allocator

Now you can access values by simple iteration over resulting document:
```
for (json_value *it = root->first_child; it; it = it->next_sibling)
{
	if (it->name)
	{
		printf("%s = ", it->name);
	}

	switch(it->type)
	{
	case JSON_NULL:
		printf("null\n");
		break;
	case JSON_STRING:
		printf("\"%s\"\n", it->string_value);
		break;
	case JSON_INT:
		printf("%d\n", it->int_value);
		break;
	case JSON_FLOAT:
		printf("%f\n", it->float_value);
		break;
	case JSON_BOOL:
		printf(it->int_value ? "true\n" : "false\n");
		break;
	}
}
```
Root node lifetime is equals to block allocator lifetime. For easy loading you maybe want write something like [SampleDocumentClass](SampleDocumentClass.md) using own file\logging functions.

# Modifying #
You can replace allocator. Just change json\_parse() signature and json\_alloc()

Or create XML SAX like parsing by adding callback to json\_parse() and call them instead of json\_append()

# License #
Distributed under the MIT license. Copyright (c) 2010, Ivan Vashchaev