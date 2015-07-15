import imagehash
from imagehashstorage import ImageHashStorage

files = [
    'images/bad-girl.jpeg',
    'images/bad-girl-with-text.jpg',
    'images/dancing-girl.jpg',
    'images/dancing-girl-with-arrow.jpg',
    'images/man.jpg',
    'images/man-cropped.jpg',
    'images/diet-will-be-started-on-tommrow.jpg',
    'images/there-is-no-reason-not-to-happy.jpg',
    'images/doctor.gif',
]

data = []

for file in files:
    item = {
        'filename': file,
        'hash': imagehash.calculate(file),
    }
    print('{0} -> {1}'.format(item['filename'], hex(item['hash'])))

    data.append(item)

storage = ImageHashStorage()
for item in data:
    print('\n#', item['filename'])

    neighbor = storage.query(item['hash'])
    if neighbor:
        print('neighbor found')
        print('id:', neighbor['id'])
        print('hash:', hex(neighbor['hash']))
    else:
        print('added')
        storage.add(item['filename'], item['hash'])