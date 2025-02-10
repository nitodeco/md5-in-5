import sys
from md5 import md5


def main():
    if len(sys.argv) < 2:
        print("Usage: python main.py <input>")
        sys.exit(1)

    input = sys.argv[1]
    hash = md5(input)

    print(hash)


if __name__ == "__main__":
    main()
