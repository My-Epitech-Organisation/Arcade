from graphviz import Digraph
# Further enhancing the UML diagram by adding inheritance details for core classes

uml = Digraph("Arcade_UML", filename="arcade_uml", format="png")

# Core Architecture
uml.node("ArcadeCore", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>ArcadeCore</B></TD></TR>
    <TR><TD ALIGN="LEFT">- _currentGame: IGameModule*</TD></TR>
    <TR><TD ALIGN="LEFT">- _currentDisplay: IDisplayModule*</TD></TR>
    <TR><TD ALIGN="LEFT">+ run()</TD></TR>
    <TR><TD ALIGN="LEFT">+ loadGame(gameName: string)</TD></TR>
    <TR><TD ALIGN="LEFT">+ loadDisplay(displayName: string)</TD></TR>
</TABLE>>""", shape="none")

uml.node("DLLoader", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>DLLoader</B></TD></TR>
    <TR><TD ALIGN="LEFT">+ load&lt;T&gt;(libPath: string): T*</TD></TR>
    <TR><TD ALIGN="LEFT">+ unload()</TD></TR>
</TABLE>>""", shape="none")

uml.node("GameLoop", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>GameLoop</B></TD></TR>
    <TR><TD ALIGN="LEFT">+ run(game: IGameModule*, display: IDisplayModule*)</TD></TR>
</TABLE>>""", shape="none")

# Interface Definitions
uml.node("ISystem", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>ISystem</B></TD></TR>
    <TR><TD ALIGN="LEFT">+ update()</TD></TR>
</TABLE>>""", shape="none")

uml.node("IGameModule", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>IGameModule</B></TD></TR>
    <TR><TD ALIGN="LEFT">+ init()</TD></TR>
    <TR><TD ALIGN="LEFT">+ update()</TD></TR>
    <TR><TD ALIGN="LEFT">+ render()</TD></TR>
    <TR><TD ALIGN="LEFT">+ getElements(): vector&lt;Entity&gt;</TD></TR>
    <TR><TD ALIGN="LEFT">+ handleInput(event: IEventManager&amp;)</TD></TR>
    <TR><TD ALIGN="LEFT">+ getName(): string</TD></TR>
    <TR><TD ALIGN="LEFT">+ getScore(): int</TD></TR>
</TABLE>>""", shape="none")

uml.node("IDisplayModule", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>IDisplayModule</B></TD></TR>
    <TR><TD ALIGN="LEFT">+ init()</TD></TR>
    <TR><TD ALIGN="LEFT">+ getInput(): IEventManager</TD></TR>
    <TR><TD ALIGN="LEFT">+ drawElement(entities: vector&lt;Entity&gt;&amp;)</TD></TR>
    <TR><TD ALIGN="LEFT">+ render()</TD></TR>
    <TR><TD ALIGN="LEFT">+ getName(): string</TD></TR>
</TABLE>>""", shape="none")

uml.node("IEventManager", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>IEventManager</B></TD></TR>
    <TR><TD ALIGN="LEFT">+ getEventState(key: int): bool</TD></TR>
    <TR><TD ALIGN="LEFT">+ setEventState(key: int, state: bool)</TD></TR>
    <TR><TD ALIGN="LEFT">+ getMouseX(): size_t</TD></TR>
    <TR><TD ALIGN="LEFT">+ getMouseY(): size_t</TD></TR>
    <TR><TD ALIGN="LEFT">+ pollEvents()</TD></TR>
</TABLE>>""", shape="none")

# ECS Components
uml.node("EntityManager", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>EntityManager</B></TD></TR>
    <TR><TD ALIGN="LEFT">- _nextEntityId: Entity</TD></TR>
    <TR><TD ALIGN="LEFT">- _activeEntities: vector&lt;Entity&gt;</TD></TR>
    <TR><TD ALIGN="LEFT">+ createEntity(): Entity</TD></TR>
    <TR><TD ALIGN="LEFT">+ destroyEntity(entity: Entity)</TD></TR>
    <TR><TD ALIGN="LEFT">+ getEntities(): vector&lt;Entity&gt;&amp;</TD></TR>
</TABLE>>""", shape="none")

uml.node("ComponentManager", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>ComponentManager&lt;T&gt;</B></TD></TR>
    <TR><TD ALIGN="LEFT">- components: unordered_map&lt;Entity, T&gt;</TD></TR>
    <TR><TD ALIGN="LEFT">+ addComponent(entity: Entity, component: T)</TD></TR>
    <TR><TD ALIGN="LEFT">+ removeComponent(entity: Entity)</TD></TR>
    <TR><TD ALIGN="LEFT">+ getComponent(entity: Entity): T*</TD></TR>
    <TR><TD ALIGN="LEFT">+ getAll(): unordered_map&lt;Entity, T&gt;&amp;</TD></TR>
</TABLE>>""", shape="none")

uml.node("SystemManager", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>SystemManager</B></TD></TR>
    <TR><TD ALIGN="LEFT">- _systems: vector&lt;ISystem*&gt;</TD></TR>
    <TR><TD ALIGN="LEFT">+ addSystem(system: ISystem*)</TD></TR>
    <TR><TD ALIGN="LEFT">+ updateSystems()</TD></TR>
</TABLE>>""", shape="none")

# Components
uml.node("PositionComponent", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>PositionComponent</B></TD></TR>
    <TR><TD ALIGN="LEFT">+ x: int</TD></TR>
    <TR><TD ALIGN="LEFT">+ y: int</TD></TR>
</TABLE>>""", shape="none")

uml.node("VelocityComponent", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>VelocityComponent</B></TD></TR>
    <TR><TD ALIGN="LEFT">+ dx: int</TD></TR>
    <TR><TD ALIGN="LEFT">+ dy: int</TD></TR>
</TABLE>>""", shape="none")

uml.node("SpriteComponent", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>SpriteComponent</B></TD></TR>
    <TR><TD ALIGN="LEFT">+ path: string</TD></TR>
</TABLE>>""", shape="none")

# Systems
uml.node("MovementSystem", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>MovementSystem</B></TD></TR>
    <TR><TD ALIGN="LEFT">- positionManager: ComponentManager&lt;PositionComponent&gt;*</TD></TR>
    <TR><TD ALIGN="LEFT">- velocityManager: ComponentManager&lt;VelocityComponent&gt;*</TD></TR>
    <TR><TD ALIGN="LEFT">+ update()</TD></TR>
</TABLE>>""", shape="none")

uml.node("AEventManager", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>AEventManager</B></TD></TR>
    <TR><TD ALIGN="LEFT">- _keyEvents: vector&lt;pair&lt;int, bool&gt;&gt;</TD></TR>
    <TR><TD ALIGN="LEFT">- _mouseX: size_t</TD></TR>
    <TR><TD ALIGN="LEFT">- _mouseY: size_t</TD></TR>
    <TR><TD ALIGN="LEFT">+ getEventState(key: int): bool</TD></TR>
    <TR><TD ALIGN="LEFT">+ setEventState(key: int, state: bool)</TD></TR>
    <TR><TD ALIGN="LEFT">+ getMouseX(): size_t</TD></TR>
    <TR><TD ALIGN="LEFT">+ getMouseY(): size_t</TD></TR>
</TABLE>>""", shape="none")

uml.node("WindowManager", label="""<
<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
    <TR><TD><B>WindowManager</B></TD></TR>
    <TR><TD ALIGN="LEFT">- _width: int</TD></TR>
    <TR><TD ALIGN="LEFT">- _height: int</TD></TR>
    <TR><TD ALIGN="LEFT">- _fps: float</TD></TR>
    <TR><TD ALIGN="LEFT">- _title: string</TD></TR>
    <TR><TD ALIGN="LEFT">+ getWindowSize(): Vector2i</TD></TR>
    <TR><TD ALIGN="LEFT">+ setWindowSize(vec: Vector2i)</TD></TR>
</TABLE>>""", shape="none")

# Relationships
# Interface implementations
uml.edge("IEventManager", "AEventManager", arrowhead="vee", label="implements")
uml.edge("ISystem", "MovementSystem", arrowhead="vee", label="implements")
uml.edge("IGameModule", "EntityManager", arrowhead="vee", label="uses")
uml.edge("IDisplayModule", "EntityManager", arrowhead="vee", label="uses")
uml.edge("IGameModule", "WindowManager", arrowhead="vee", label="uses")
uml.edge("IDisplayModule", "WindowManager", arrowhead="vee", label="uses")

# Core relationships
uml.edge("ArcadeCore", "DLLoader", arrowhead="vee", label="uses")
uml.edge("ArcadeCore", "GameLoop", arrowhead="vee", label="uses")
uml.edge("ArcadeCore", "IGameModule", arrowhead="vee", label="manages")
uml.edge("ArcadeCore", "IDisplayModule", arrowhead="vee", label="manages")
uml.edge("GameLoop", "IGameModule", arrowhead="vee", label="uses")
uml.edge("GameLoop", "IDisplayModule", arrowhead="vee", label="uses")

# ECS relationships
uml.edge("ComponentManager", "EntityManager", arrowhead="vee", label="uses")
uml.edge("SystemManager", "ISystem", arrowhead="vee", label="manages")
uml.edge("MovementSystem", "ComponentManager", arrowhead="vee", label="uses")

# Component relationships
uml.edge("ComponentManager", "PositionComponent", arrowhead="vee", label="manages")
uml.edge("ComponentManager", "VelocityComponent", arrowhead="vee", label="manages")
uml.edge("ComponentManager", "SpriteComponent", arrowhead="vee", label="manages")

# Event relationships
uml.edge("IGameModule", "IEventManager", arrowhead="vee", label="uses")
uml.edge("IDisplayModule", "IEventManager", arrowhead="vee", label="provides")

# Render and display the UML diagram
uml.render("arcade_uml", cleanup=True)
print("UML diagram has been generated as arcade_uml.png")