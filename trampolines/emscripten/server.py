import os
import posixpath
import urllib

import SimpleHTTPServer
import SocketServer

PORT = 8000

ROUTES = (
    ['/assets', '../../examples/assets'],
    ['', '.']
) 

class MyRequestHandler(SimpleHTTPServer.SimpleHTTPRequestHandler):
    def translate_path(self, path):
        """translate path given routes"""

        # set default root to cwd
        root = os.getcwd()
        
        # look up routes and set root directory accordingly
        for pattern, rootdir in ROUTES:
            if path.startswith(pattern):
                path = path[len(pattern):]  # consume path up to pattern len
                root = rootdir
                break
        
        # normalize path and prepend root directory
        path = path.split('?',1)[0]
        path = path.split('#',1)[0]
        path = posixpath.normpath(urllib.unquote(path))

        words = path.split('/')
        words = filter(None, words)

        path = root
        for word in words:
            drive, word = os.path.splitdrive(word)
            head, word = os.path.split(word)
            if word in (os.curdir, os.pardir):
                continue
            path = os.path.join(path, word)
 
        return path


if __name__ == '__main__':    
    Handler = MyRequestHandler
    httpd = SocketServer.TCPServer(("", PORT), Handler)
    print "Serving on port", PORT
    httpd.serve_forever()

