from subprocess import Popen, PIPE
import os
import sys

MONGA_PATH = '../bin/builds/monga/monga'
LEX_CASES_PATH = 'lexcases'
YACC_CASES_PATH = 'yacccases'

def test_file(program_args, input_file_path):
	# Get Monga output for input file
	process = Popen(program_args, stdout=PIPE, universal_newlines=True)
	(output, _) = process.communicate()
	process.wait()

	output = output.rstrip()

	# Read expected output file
	expected_out = ""
	with open(input_file_path + ".expected", "r") as expected_file_handle:
		expected_out = expected_file_handle.read()
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

def test_yacc(monga_path, input_file_path):
	test_file([monga_path, '-p', input_file_path], input_file_path)

def test_all_lex(monga_path, lex_cases_path):
	print('Testing Lex...')

	for file in os.listdir(lex_cases_path):
		if not file.endswith(".expected"):
			input_file_path = os.path.join(lex_cases_path, file)
			test_lex(monga_path, input_file_path)

	print('Lex tests finished.')

def test_all_yacc(monga_path, yacc_cases_path):
	print('Testing Yacc...')

	for file in os.listdir(yacc_cases_path):
		if not file.endswith(".expected"):
			input_file_path = os.path.join(yacc_cases_path, file)
			test_yacc(monga_path, input_file_path)

	print('Yacc tests finished.')

def test_all(monga_path):
	test_all_lex(monga_path, LEX_CASES_PATH)
	test_all_yacc(monga_path, YACC_CASES_PATH)

test_all(MONGA_PATH)