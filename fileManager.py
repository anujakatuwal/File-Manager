import os

class createNode:
    def __init__(self, filename):
        self.filename = filename
        self.left = None
        self.right = None
        self.height = 1

def max(a, b):
    return a if a > b else b

def getHeight(root):
    if root is None:
        return 0
    return root.height

def getBalanceFactor(root):
    if root is None:
        return 0
    return getHeight(root.left) - getHeight(root.right)

def rightRotate(y):
    x = y.left
    T2 = x.right
    
    x.right = y
    y.left = T2

    y.height = max(getHeight(y.right), getHeight(y.left)) + 1
    x.height = max(getHeight(x.right), getHeight(x.left)) + 1

    return x

def leftRotate(x):
    y = x.right
    T2 = y.left

    y.left = x
    x.right = T2

    x.height = max(getHeight(x.right), getHeight(x.left)) + 1
    y.height = max(getHeight(y.right), getHeight(y.left)) + 1

    return y

def insert(root, filename):
    if root is None:
        return createNode(filename)

    if filename == root.filename:
        return root
    elif filename < root.filename:
        root.left = insert(root.left, filename)
    else:
        root.right = insert(root.right, filename)

    root.height = max(getHeight(root.right), getHeight(root.left)) + 1
    bf = getBalanceFactor(root)

    # Left Left Case
    if bf > 1 and filename < root.left.filename:
        return rightRotate(root)

    # Right Right Case
    if bf < -1 and filename > root.right.filename:
        return leftRotate(root)

    # Left Right Case
    if bf > 1 and filename > root.left.filename:
        root.left = leftRotate(root.left)
        return rightRotate(root)

    # Right Left Case
    if bf < -1 and filename < root.right.filename:
        root.right = rightRotate(root.right)
        return leftRotate(root)

    return root

def findMin(root):
    while root.left is not None:
        root = root.left
    return root

def delete(root, filename):
    if root is None:
        return root

    if filename < root.filename:
        root.left = delete(root.left, filename)
    elif filename > root.filename:
        root.right = delete(root.right, filename)
    else:
        if root.left is None:
            temp = root.right
            root = None
            return temp
        elif root.right is None:
            temp = root.left
            root = None
            return temp
        else:
            temp = findMin(root.right)
            root.filename = temp.filename
            root.right = delete(root.right, temp.filename)


def search(root, filename):
    if root is None or root.filename == filename:
        return root
    if filename < root.filename:
        return search(root.left, filename)
    return search(root.right, filename)

def in_order(root):
    if root is not None:
        in_order(root.left)
        print(root.filename)
        in_order(root.right)

def scanDirectory(root, dirName):
    try:
        print(f"You are inside {dirName}")
        for entry in os.listdir(dirName):
            if not entry.startswith('.'):
                root = insert(root, entry)
    except FileNotFoundError:
        print(f"Could not open directory: {dirName}")
    return root

def main():
    root = None
    filename = ""
    dirName = input("Enter directory path: ")

    while not os.path.isdir(dirName):
        print(f"Could not open directory: {dirName}")
        dirName = input("Please enter a valid directory path: ")

    root = scanDirectory(root, dirName)

    while True:
        print("\n1.CREATE FILE\n2.DELETE FILE\n3.SEARCH FILE\n4.DISPLAY ALL FILES\n5.EXIT")
        choice = int(input("Enter your choice: "))

        if choice == 1:
            filename = input("Enter the filename to be created: ")
            path = os.path.join(dirName, filename)

            while os.path.exists(path): 
                print(f"{filename} already exists.")
                filename = input("Please enter a new filename: ")  
                path = os.path.join(dirName, filename)

            with open(path, "w") as file:
                root = insert(root, filename)
                print(f"{filename} created in {dirName}")

        elif choice == 2:
            filename = input("Enter the filename to be deleted: ")
            path = os.path.join(dirName, filename)

            if os.path.exists(path):
                os.remove(path)
                root = delete(root, filename)
                print(f"{filename} deleted from {dirName}")
            else:
                print("File not found or deletion failed.")

        elif choice == 3:
            filename = input("Enter the filename to be searched: ")
            temp = search(root, filename)
            if temp:
                print(f"{filename} found")
            else:
                print("File not found")

        elif choice == 4:
            print("\nFiles in sorted order:")
            in_order(root)

        elif choice == 5:
            print("Exiting program...")
            return

        else:
            print("Invalid choice. Try again.")

if __name__ == "__main__":
    main()