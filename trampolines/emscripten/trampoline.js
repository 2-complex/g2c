

var MouseController = function(canvas, program)
{
    this.canvas = canvas;
    this.isMouseDown = false;

    canvas.onmousedown = this.onMouseDown.bind(this);
    canvas.onmouseup = this.onMouseUp.bind(this);
    canvas.onmousemove = this.onMouseMove.bind(this);
    canvas.onmouseleave = this.onMouseLeave.bind(this);
}

MouseController.prototype.getLocalCoord = function(theEvent)
{
    return { x: theEvent.clientX, y: this.canvas.height - theEvent.clientY };
}

MouseController.prototype.onMouseDown = function(theEvent)
{
    var e = this.getLocalCoord(theEvent);
    this.isMouseDown = true;
    Bindings.mouseDown(e.x, e.y);
}

MouseController.prototype.onMouseUp = function(theEvent)
{
    var e = this.getLocalCoord(theEvent);
    this.isMouseDown = false;
    Bindings.mouseUp(e.x, e.y);
}

MouseController.prototype.onMouseMove = function(theEvent)
{
    var e = this.getLocalCoord(theEvent);

    if( this.isMouseDown )
    {
        Bindings.mouseDragged(e.x, e.y);
    }
}

MouseController.prototype.onMouseLeave = function(theEvent)
{
    this.isMouseDown = false;
}

// write javascript float arrays to the emscripten heap
var HeapUtils = function() {}

HeapUtils.floatArrayToHeap = function(arr)
{
    var arrayPointer = _malloc(arr.length * 4);
    for (var i = 0; i < arr.length; i++)
        Module.setValue(arrayPointer + i * 4, arr[i], 'float');
    return arrayPointer;
}


var Bindings = function Bindings() {}

Bindings.init = Module.cwrap("init", "", []);
Bindings.resize = Module.cwrap("resize", "", ["number", "number"]);
Bindings.step = Module.cwrap("step", "", ["number"]);
Bindings.draw = Module.cwrap("draw", "", []);

Bindings.mouseDown = Module.cwrap("mouseDown", "", ["number", "number"]);
Bindings.mouseDragged = Module.cwrap("mouseDragged", "", ["number", "number"]);
Bindings.mouseUp = Module.cwrap("mouseUp", "", ["number", "number"]);


var Program = function Program(canvas)
{
    this.canvas = canvas;
    this.translation = { originX: 0, originY: 0, zoom: 1.0 };

    Bindings.resize(canvas.width, canvas.height);
    Bindings.init();

    this.mouseController = new MouseController(canvas, this);

    this.invalidate();
}


Program.prototype.render = function()
{
    Bindings.step((new Date).getTime() / 1000.0);
    Bindings.draw();

    this.invalidate();
}

Program.prototype.invalidate = function()
{
    window.requestAnimationFrame(this.render.bind(this));
}


