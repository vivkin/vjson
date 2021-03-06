#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "json.h"

void populate_sources(const char *filter, std::vector<std::vector<char> > &sources)
{
	char filename[256];
	for (int i = 1; i < 64; ++i)
	{
		sprintf(filename, filter, i);
		FILE *fp = fopen(filename, "rb");
		if (fp)
		{
			fseek(fp, 0, SEEK_END);
			size_t size = (size_t)ftell(fp);
			fseek(fp, 0, SEEK_SET);
			std::vector<char> buffer(size + 1);
			if (fread(&buffer[0], 1, size, fp) != size)
			{
				exit(EXIT_FAILURE);
			}
			fclose(fp);
			sources.push_back(buffer);
		}
		else
		{
			break;
		}
	}

	printf("Loaded %zd json files\n", sources.size());
}

#define INDENT(n) for (int i = 0; i < n; ++i) printf("    ")

void print(json_value *value, int indent = 0)
{
	INDENT(indent);
	if (value->name) printf("\"%s\" = ", value->name);
	switch(value->type)
	{
	case JSON_NULL:
		printf("null\n");
		break;
	case JSON_OBJECT:
	case JSON_ARRAY:
		printf(value->type == JSON_OBJECT ? "{\n" : "[\n");
		for (json_value *it = value->first_child; it; it = it->next_sibling)
		{
			print(it, indent + 1);
		}
		INDENT(indent);
		printf(value->type == JSON_OBJECT ? "}\n" : "]\n");
		break;
	case JSON_STRING:
		printf("\"%s\"\n", value->string_value);
		break;
	case JSON_INT:
		printf("%d\n", value->int_value);
		break;
	case JSON_FLOAT:
		printf("%f\n", value->float_value);
		break;
	case JSON_BOOL:
		printf(value->int_value ? "true\n" : "false\n");
		break;
	}
}

bool parse(char *source)
{
	char *errorPos = 0;
	const char *errorDesc = 0;
	int errorLine = 0;
	block_allocator allocator(1 << 10);
	
	json_value *root = json_parse(source, &errorPos, &errorDesc, &errorLine, &allocator);
	if (root)
	{
		print(root);
		return true;
	}

	printf("Error at line %d: %s\n%s\n\n", errorLine, errorDesc, errorPos);
	return false;
}

int main()
{
	// Fail
	printf("===FAIL===\n\n");
	std::vector<std::vector<char> > sources;
	populate_sources("test/fail%d.json", sources);
	int passed = 0;
	for (size_t i = 0; i < sources.size(); ++i)
	{
		printf("Parsing %zd\n", i + 1);
		if (parse(&sources[i][0]))
		{
			++passed;
		}
	}
	printf("Passed %d from %zd tests\n", passed, sources.size());

	// Pass
	sources.clear();
	printf("\n===PASS===\n\n");
	populate_sources("test/pass%d.json", sources);
	passed = 0;
	for (size_t i = 0; i < sources.size(); ++i)
	{
		printf("Parsing %zd\n", i + 1);
		if (parse(&sources[i][0]))
		{
			++passed;
		}
	}
	printf("Passed %d from %zd tests\n", passed, sources.size());

	return 0;
}
