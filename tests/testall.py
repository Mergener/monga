# Run this file from the ./testall script located at the repository's top level directory.

from subprocess import Popen, PIPE, STDOUT
import os
import sys

if (len(sys.argv) != 2):
	print(f"Usage: {sys.argv[0]} <pathToMongaExecutable>")
	exit(-1)

MONGA_PATH = sys.argv[1]
CASES_PATH = os.path.dirname(os.path.realpath(__file__))
os.chdir(os.path.join(CASES_PATH, ".."))

# Number of passed tests
passed = 0
# Number of finished tests
done = 0

def test_cmd(cmd, expected_out_path):
	global passed
	global done

	# Get Monga output for input file
	process = Popen(cmd, stdout=PIPE, stderr=STDOUT, universal_newlines=True)
	(output, _) = process.communicate()
	process.wait()

	output = output.rstrip()

	# Read expected output file
	expected_out = ""
	try:
		with open(expected_out_path, "r") as expected_file_handle:
			expected_out = expected_file_handle.read()
	except:
		print(f"Couldn't find expected output file '{expected_out_path}'.")
		return

	expected_out = expected_out.rstrip()

	# Compare two outputs:
	equal = expected_out == output

	testCaseCmd = ' '.join(cmd)

	done += 1

	if equal:
		passed += 1
	else:
		print(f"[Test] {testCaseCmd} (FAILED)\n-> Expected:\n[{expected_out}]\n-> Got:\n[{output}]")

def test_lex(input_file_path, expected_out_path):
	global MONGA_PATH
	test_cmd([MONGA_PATH, '-l', input_file_path], expected_out_path)

def test_astdump(input_file_path, expected_out_path):
	global MONGA_PATH
	test_cmd([MONGA_PATH, '-p', input_file_path], expected_out_path)

def test_sem(input_file_path, expected_out_path):
	global MONGA_PATH
	target = []
	if os.path.isdir(input_file_path):
		for file in os.listdir(input_file_path):
			if not os.path.isdir(file):
				target.append(os.path.join(input_file_path, file))
	else:
		target.append(input_file_path)

	test_cmd([MONGA_PATH, '-s'] + target, expected_out_path)
	
def test_reducedump(input_file_path, expected_out_path):
	global MONGA_PATH
	test_cmd([MONGA_PATH, '-r', input_file_path], expected_out_path)

def test_each(path, test_name, test_func):
	global done
	global passed

	print(f"\n*** Starting '{test_name}' tests ***")

	done = 0
	passed = 0

	for file in os.listdir(path):
		if not file.endswith(".expected"):
			input_file_path = os.path.join(path, file)
			test_func(input_file_path, input_file_path + ".expected")
	
	print(f"\n*** '{test_name}' tests finished. ({passed} of {done} passed) ***")

def test_all():
	global MONGA_PATH

	test_each(os.path.join("tests", "lex_cases"), "Lex Dump", test_lex)
	test_each(os.path.join("tests", "reduce_dump_cases"), "Rule Reduce Dump", test_reducedump)
	test_each(os.path.join("tests", "ast_dump_cases"), "AST Dump", test_astdump)
	test_each(os.path.join("tests", "sem_test_cases"), "Semantic", test_sem)

test_all()

if done == passed:
	exit(0)
exit(-1)