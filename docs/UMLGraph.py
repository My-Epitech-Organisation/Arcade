# Enhanced UML Class Diagram for the Arcade project with more precision
from graphviz import Digraph
# Further enhancing the UML diagram by adding inheritance details for core classes

uml = Digraph("Arcade_UML", filename="arcade_uml", format="png")

# Define Interfaces
uml.node("IGame", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><I>IGame</I></TD></TR>
    <TR><TD ALIGN="LEFT">+ start()</TD></TR>
    <TR><TD ALIGN="LEFT">+ update()</TD></TR>
    <TR><TD ALIGN="LEFT">+ render()</TD></TR>
    <TR><TD ALIGN="LEFT">+ handleInput()</TD></TR>
</TABLE>>""", shape="none")

uml.node("IRenderer", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><I>IRenderer</I></TD></TR>
    <TR><TD ALIGN="LEFT">+ draw()</TD></TR>
    <TR><TD ALIGN="LEFT">+ clearScreen()</TD></TR>
    <TR><TD ALIGN="LEFT">+ updateDisplay()</TD></TR>
</TABLE>>""", shape="none")

# Define Abstract Classes
uml.node("AbstractGame", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>AbstractGame</B></TD></TR>
    <TR><TD ALIGN="LEFT"><I>inherits from IGame</I></TD></TR>
    <TR><TD ALIGN="LEFT">+ start()</TD></TR>
    <TR><TD ALIGN="LEFT">+ update()</TD></TR>
    <TR><TD ALIGN="LEFT">+ render()</TD></TR>
</TABLE>>""", shape="none")

uml.node("AbstractRenderer", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>AbstractRenderer</B></TD></TR>
    <TR><TD ALIGN="LEFT"><I>inherits from IRenderer</I></TD></TR>
    <TR><TD ALIGN="LEFT">+ draw()</TD></TR>
    <TR><TD ALIGN="LEFT">+ clearScreen()</TD></TR>
</TABLE>>""", shape="none")

# Define Factories
uml.node("GameFactory", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>GameFactory</B></TD></TR>
    <TR><TD ALIGN="LEFT">+ createGame(type: string): unique_ptr&lt;IGame&gt;</TD></TR>
</TABLE>>""", shape="none")

uml.node("RendererFactory", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>RendererFactory</B></TD></TR>
    <TR><TD ALIGN="LEFT">+ createRenderer(type: string): unique_ptr&lt;IRenderer&gt;</TD></TR>
</TABLE>>""", shape="none")

# Core System
uml.node("ArcadeCore", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>ArcadeCore</B></TD></TR>
    <TR><TD ALIGN="LEFT"><B>- _currentGame</B>: unique_ptr&lt;IGame&gt;</TD></TR>
    <TR><TD ALIGN="LEFT"><B>- _currentRenderer</B>: unique_ptr&lt;IRenderer&gt;</TD></TR>
    <TR><TD ALIGN="LEFT">+ loadGame(game: string)</TD></TR>
    <TR><TD ALIGN="LEFT">+ loadRenderer(renderer: string)</TD></TR>
    <TR><TD ALIGN="LEFT">+ mainLoop()</TD></TR>
</TABLE>>""", shape="none")

# Plugin Loader
uml.node("PluginLoader", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>PluginLoader</B></TD></TR>
    <TR><TD ALIGN="LEFT">+ loadLibrary(filename: string): void*</TD></TR>
    <TR><TD ALIGN="LEFT">+ getSymbol(handle: void*, symbol: string): void*</TD></TR>
</TABLE>>""", shape="none")

# Event Manager (Observer Pattern)
uml.node("EventManager", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>EventManager</B></TD></TR>
    <TR><TD ALIGN="LEFT">+ registerObserver(observer: IObserver)</TD></TR>
    <TR><TD ALIGN="LEFT">+ notify(event: EventType)</TD></TR>
</TABLE>>""", shape="none")

# Score Management (Singleton)
uml.node("ScoreManager", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>ScoreManager</B></TD></TR>
    <TR><TD ALIGN="LEFT">+ saveScore(player: string, score: int)</TD></TR>
    <TR><TD ALIGN="LEFT">+ getScores(): vector&lt;pair&lt;string, int&gt;&gt;</TD></TR>
</TABLE>>""", shape="none")

# ECS - Entity Component System
uml.node("ECS", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>Entity Component System</B></TD></TR>
    <TR><TD ALIGN="LEFT">+ addEntity()</TD></TR>
    <TR><TD ALIGN="LEFT">+ addComponent(entity, component)</TD></TR>
    <TR><TD ALIGN="LEFT">+ removeComponent(entity, component)</TD></TR>
    <TR><TD ALIGN="LEFT">+ update()</TD></TR>
</TABLE>>""", shape="none")

# Define concrete implementations for games and renderers
uml.node("SnakeGame", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>SnakeGame</B></TD></TR>
    <TR><TD ALIGN="LEFT"><I>inherits from AbstractGame</I></TD></TR>
</TABLE>>""", shape="none")

uml.node("PacmanGame", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>PacmanGame</B></TD></TR>
    <TR><TD ALIGN="LEFT"><I>inherits from AbstractGame</I></TD></TR>
</TABLE>>""", shape="none")

uml.node("SFMLRenderer", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>SFMLRenderer</B></TD></TR>
    <TR><TD ALIGN="LEFT"><I>inherits from AbstractRenderer</I></TD></TR>
</TABLE>>""", shape="none")

uml.node("SDLRenderer", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>SDLRenderer</B></TD></TR>
    <TR><TD ALIGN="LEFT"><I>inherits from AbstractRenderer</I></TD></TR>
</TABLE>>""", shape="none")

# Relationships
uml.edge("IGame", "AbstractGame", arrowhead="empty", label="inherits")
uml.edge("IRenderer", "AbstractRenderer", arrowhead="empty", label="inherits")

uml.edge("AbstractGame", "SnakeGame", arrowhead="empty", label="inherits")
uml.edge("AbstractGame", "PacmanGame", arrowhead="empty", label="inherits")
uml.edge("AbstractRenderer", "SFMLRenderer", arrowhead="empty", label="inherits")
uml.edge("AbstractRenderer", "SDLRenderer", arrowhead="empty", label="inherits")

uml.edge("GameFactory", "AbstractGame", arrowhead="none", label="creates")
uml.edge("RendererFactory", "AbstractRenderer", arrowhead="none", label="creates")
uml.edge("ECS", "AbstractGame", arrowhead="none", label="used by")

uml.edge("ArcadeCore", "AbstractGame", arrowhead="none", label="manages")
uml.edge("PluginLoader", "AbstractGame", arrowhead="none", label="loads")
uml.edge("PluginLoader", "AbstractRenderer", arrowhead="none", label="loads")
uml.edge("ArcadeCore", "AbstractRenderer", arrowhead="none", label="manages")

uml.edge("EventManager", "AbstractGame", arrowhead="none", label="notifies")
uml.edge("ScoreManager", "ArcadeCore", arrowhead="none", label="tracks scores")

# Render and display the UML diagram
uml_path = "arcade_uml"
uml.render(uml_path, format="png", cleanup=True)
uml_path
