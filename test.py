import random
import subprocess
import string
import os
import filecmp

def str_generator(size, chars=string.ascii_lowercase + string.ascii_uppercase + string.digits):
    return ''.join(random.choice(chars) for _ in range(size))

class PythonTest:
    out_suffix = '_OUT'
    def __init__(self):
        pass

    def _create_random_test(self):
        # integer range
        magn = 1000000
        number = random.randint(-magn, magn)
        string = str_generator(random.randint(1,80))
        filename = str_generator(random.randint(1,80))
        # file content
        content = str_generator(random.randint(1,magn))

        with open(filename, 'wb') as f_test:
            f_test.write(content)

        return number, string, filename, content

    def _clean_random_test(self, filename):
        if os.path.exists(filename):
            os.remove(filename)

        if os.path.exists(filename + PythonTest.out_suffix):
            os.remove(filename + PythonTest.out_suffix)

    # return 0 if everything is fine; 1 otherwise
    def _test(self, number, string, filename, content, net_addr = r"127.0.0.1:50005"):
        server = subprocess.Popen(["./server_app", net_addr, str(number), string, filename], stdout=subprocess.PIPE)
        client = subprocess.Popen(["./client_app", net_addr, filename + PythonTest.out_suffix], stdout=subprocess.PIPE)

        out_client, _ = client.communicate()
        server.terminate()
        out_server, _ = server.communicate()

        matches = 0
        for s1, s2 in zip(out_client.split('\n'), out_server.split('\n')):
            if ':' in s1 and ':' in s2:
                # check if server send the same that cliend received
                if s1[s1.index(':') + 1:] == s2[s2.index(':') + 1:]:
                    matches += 1

        if matches != 4:
            return 1

        # check if file is the same
        try:
            # only when we have file to sent
            if os.path.exists(filename):
                if not filecmp.cmp(filename, filename + PythonTest.out_suffix):
                    return 1
        except:
            return 1

        return 0

    # return 0 if everything is fine; 1 otherwise
    def test_random(self):
        number, string, filename, content = self._create_random_test()
        result = self._test(number, string, filename, content)
        self._clean_random_test(filename)
        return result

    # return 0 if everything is fine; 1 otherwise
    def test_empty(self):
        return self._test(0, "", "", "")



if __name__ == "__main__":
    test = PythonTest()

    if test.test_empty() == 0:
        print("empty test : correct")
    else:
        print("empty test : wrong")

    for i in range(0,10):
        if test.test_random() == 0:
            print("random test : correct")
        else:
            print("random test : wrong")
