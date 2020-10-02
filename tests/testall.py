from subprocess import Popen, PIPE
import os
import sys

monga_path = "../bin/monga"
cases_path = "cases"

def test_file(program_args, input_file_path):
	# Get Monga output for input file
	process = Popen(program_args, stdout=PIPE, universal_newlines=True)
	(output, _) = process.communicate()
	process.wait()

	output = output[:-1]

	# Read expected output file
	expected_out = ""
	with open(input_file_path + ".expected", "r") as expected_file_handle:
		expected_out = expected_file_handle.read()

	# Compare two outputs:
	equal = expected_out == output

	if equal:
		print(f"[Test] {input_file_path} (SUCCESS)")
	else:
		print(f"[Test] {input_file_path} (FAILED)\n-> Expected:\n[{expected_out}]\n-> Got:\n[{output}]")

def test_lex(input_file_path):
	test_file([monga_path, '-l', input_file_path], input_file_path)

def test_all():
	for file in os.listdir(cases_path):
		if not file.endswith(".expected"):
			input_file_path = os.path.join(cases_path, file)
			test_lex(input_file_path)

test_all()