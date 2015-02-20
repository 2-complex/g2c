

var MouseController = function (canvas, area)
{
    this.canvas = canvas;
    this.area = area;
    this.isMouseDown = false;
    this.lastMouseCoord = null;
    this.zoomSensitivity = 2.0;
    $(canvas).mousewheel(this.onMouseWheel.bind(this));
    canvas.onmousedown = this.onMouseDown.bind(this);
    canvas.onmouseup = this.onMouseUp.bind(this);
    canvas.onmousemove = this.onMouseMove.bind(this);
    canvas.onmouseleave = this.onMouseLeave.bind(this);
}

MouseController.prototype.getLocalCoord = function (ev)
{
    return { x: ev.clientX - this.canvas.offsetLeft, y: ev.clientY - this.canvas.offsetTop };
}

MouseController.prototype.onMouseDown = function (ev)
{
    var e = this.getLocalCoord(ev);
    this.isMouseDown = true;
    this.lastMouseCoord = e;
}

MouseController.prototype.onMouseUp = function (ev)
{
    var e = this.getLocalCoord(ev);
    this.isMouseDown = false;
    this.lastMouseCoord = null;
}

MouseController.prototype.onMouseMove = function (ev)
{
    var e = this.getLocalCoord(ev);
    if (this.isMouseDown)
    {
        var offset = { x: e.x - this.lastMouseCoord.x, y: e.y - this.lastMouseCoord.y };
        this.lastMouseCoord = e;
        this.area.pan(offset);
    }
}

MouseController.prototype.onMouseLeave = function (ev)
{
    this.isMouseDown = false;
}

MouseController.prototype.onMouseWheel = function (ev)
{
    ev.wheelDelta = ev.deltaY * 120.0;
    var e = this.getLocalCoord(ev);
    this.area.zoom(1.0 + ev.wheelDelta / 1200.0 * this.zoomSensitivity, e);
    return false;
}


// write javascript float arrays to the emscripten heap
var HeapUtils = function() {}

HeapUtils.floatArrayToHeap = function (arr)
{
    var arrayPointer = _malloc(arr.length * 4);
    for (var i = 0; i < arr.length; i++)
        Module.setValue(arrayPointer + i * 4, arr[i], 'float');
    return arrayPointer;
}


var Bindings = function Bindings() {}

Bindings.mouseDown = Module.cwrap("mouseDown", "", ["number", "number"]);
Bindings.mouseDragged = Module.cwrap("mouseDragged", "", ["number", "number"]);
Bindings.mouseUp = Module.cwrap("mouseUp", "", ["number", "number"]);

Bindings.init = Module.cwrap("init", "", []);
Bindings.resize = Module.cwrap("resize", "", ["number", "number"]);
Bindings.step = Module.cwrap("step", "", ["number"]);
Bindings.draw = Module.cwrap("draw", "", []);

Bindings.initGL = Module.cwrap('initGL', 'number', ['number', 'number']);


var Program = function Program(canvas)
{
    this.canvas = canvas;
    this.translation = { originX: 0, originY: 0, zoom: 1.0 };

    Bindings.resize(canvas.width, canvas.height);

    this.mouseController = new MouseController(canvas, this);

    this.invalidate();
}

Program.prototype.pan = function (offset)
{
    var glOffset = {
        x: offset.x / this.canvas.width * 2.0 / this.translation.zoom,
        y: offset.y / this.canvas.height * 2.0 / this.translation.zoom
    };
    this.translation.originX += glOffset.x;
    this.translation.originY -= glOffset.y;
}

Program.prototype.zoom = function (ratio, origin)
{
    this.translation.zoom *= ratio;
}

Program.prototype.render = function ()
{
    Bindings.step((new Date).getTime() / 1000.0);
    Bindings.draw();

    this.invalidate();
}

Program.prototype.invalidate = function ()
{
    window.requestAnimationFrame(this.render.bind(this));
}


