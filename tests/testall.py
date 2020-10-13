# Run this file from the ./testall script located at the repository's top level directory.

from subprocess import Popen, PIPE, STDOUT
import os
import sys

MONGA_PATH = '../bin/builds/monga/release/monga'
LEX_CASES_PATH = 'lex_cases'
AST_DUMP_CASES_PATH = 'ast_dump_cases'
REDUCE_DUMP_CASES_PATH = 'reduce_dump_cases'

def test_file(program_args, input_file_path):
	# Get Monga output for input file
	process = Popen(program_args, stdout=PIPE, stderr=STDOUT, universal_newlines=True)
	(output, _) = process.communicate()
	process.wait()

	output = output.rstrip()

	# Read expected output file
	expected_out = ""
	try:
		with open(input_file_path + ".expected", "r") as expected_file_handle:
			expected_out = expected_file_handle.read()
	except:
		print(f"Couldn't find '.expected' file for '{input_file_path}'")
		return

	expected_out = expected_out.rstrip()

	# Compare two outputs:
	equal = expected_out == output

	testCaseCmd = ' '.join(program_args)

	if equal:
		print(f"[Test] {testCaseCmd} (SUCCESS)")
	else:
		print(f"[Test] {testCaseCmd} (FAILED)\n-> Expected:\n[{expected_out}]\n-> Got:\n[{output}]")

def test_lex(monga_path, input_file_path):
	test_file([monga_path, '-l', input_file_path], input_file_path)

def test_astdump(monga_path, input_file_path):
	test_file([monga_path, '-p', input_file_path], input_file_path)
	
def test_reducedump(monga_path, input_file_path):
	test_file([monga_path, '-r', input_file_path], input_file_path)

def test_all_lex(monga_path, lex_cases_path):
	print('Testing Ast Dump...')

	for file in os.listdir(lex_cases_path):
		if not file.endswith(".expected"):
			input_file_path = os.path.join(lex_cases_path, file)
			test_lex(monga_path, input_file_path)

	print('Lex tests finished.')

def test_all_astdump(monga_path, astdump_cases_path):
	print('Testing Ast Dumping...')

	for file in os.listdir(astdump_cases_path):
		if not file.endswith(".expected"):
			input_file_path = os.path.join(astdump_cases_path, file)
			test_astdump(monga_path, input_file_path)

	print('Ast Dumping tests finished.')

def test_all_reducedump(monga_path, astdump_cases_path):
	print('Testing Parser Reduction Dumping...')

	for file in os.listdir(astdump_cases_path):
		if not file.endswith(".expected"):
			input_file_path = os.path.join(astdump_cases_path, file)
			test_reducedump(monga_path, input_file_path)

	print('Reduction Dumping tests finished.')

def test_all(monga_path):
	test_all_lex(monga_path, LEX_CASES_PATH)
	test_all_reducedump(monga_path, REDUCE_DUMP_CASES_PATH)
	#test_all_astdump(monga_path, AST_DUMP_CASES_PATH)

test_all(MONGA_PATH)