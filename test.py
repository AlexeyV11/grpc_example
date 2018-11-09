import random

class PythonTest:
    def __init__(self):

    def str_generator(size, chars=string.ascii_lowercase + string.ascii_uppercase + string.digits):
        return ''.join(random.choice(chars) for _ in range(size))

    def _create_random_test(self):
        magn = 1000000
        number = random.randint(-magn, magn)
        string = str_generator(random.randint(1,80))
        filename = str_generator(random.randint(1,80))
        content = str_generator(random.randint(1,magn))

        with open(self._filename, 'wb') as f_test:
            f_test.write(self._content)

        return number, string, filename, content


    def _clean_random_test(self, filename):
        if os.path.exists(filename):
            os.remove(filename)

        return number, string, filename, content

    def _test(self, number, string, filename, content):


    def test_random(self):
        number, string, filename, content = self._create_random_test()
        self._test(number, string, filename, content)
        self._clean_random_test(filename)

    def test_empty(self):
        self._test(0, "", "", "")



if __name__ == "__main__":
    test = PythonTest()

    test.test_empty()
    for i in range(0,10):
        test.test_random()