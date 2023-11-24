# SOFA

## Main.scn
```xml
<?xml version="1.0" ?>
<Node name="root" dt="0.005" gravity="0 0 0" >
	<AddDataRepository path="."/>
	<AddDataRepository path="../../shared/mesh"/>
    <RequiredPlugin name="SofaOpenglVisual"/>
    <VisualStyle displayFlags="hideBehaviorModels hideCollisionModels hideMappings hideForceFields" />
	<RequiredPlugin name='SofaHaptics'/>
	<RequiredPlugin name='MultiDistanceContact'/>
    
    <DefaultPipeline name="pipeline" depth="6" verbose="0"/>
    <BruteForceDetection name="detection" />
	<DefaultContactManager name="response" response="MultiFrictionContact" />
    <MultiLocalMinDistance name="proximity" alarmDistance="1.0" contactDistance="0.05" secondAlarmDistance="1.0" secondContactDistance="0.38" angleCone="0.0" />
    <FreeMotionAnimationLoop/>
	<GenericConstraintSolver tolerance="0.001" maxIt="1000"/>
    
	<InnerInstrumentSwitch name="innerInstrumentSwitch" positionAdd="-10.0 4.9 -0.9 0 -0.707107 0 0.707107" zEndoscopeAdd = "7.0" zTubeAdd = "7.0"/>
	
    <Node name="Spine" >
        <Node name="VisualModel" >
            <OglModel name="L45Spline" fileMesh="L45_mirror0.obj" scale="0.1" dy="1" /> 
        </Node>
        <Node name="CollisionModel" >
            <MeshObjLoader filename="L45_mirror00_sofa.obj"  name="loader" />
            <MeshTopology src="@loader" />
            <MechanicalObject src="@loader" name="state" scale="0.1" dy="1" />        
            <TriangleCollisionModel name="cmT" group="0" contactStiffness="100" simulated="0" moving="0"/>
            <LineCollisionModel name="cmL" group="0" contactStiffness="100" simulated="0" moving="0"/>    
        </Node>
    </Node>

    <Node name="Instrument" >
        <EulerImplicitSolver name="ODE solver" rayleighStiffness="0.05" rayleighMass="1.0" />
        <CGLinearSolver name="linear solver" iterations="25" tolerance="1e-10" threshold="10e-10" />     
		<MechanicalObject name="instrumentState0" template="Rigid3d" position="@innerInstrumentSwitch.positionAdd" showObject="1" showObjectScale="2" tags="instrument0"/>
		<Node name="Mapping">
			<MechanicalObject name="instrumentState" template="Rigid3d" position="0 0 0 0 0 0 1" showObject="0" showObjectScale="1" />
			<RigidRigidMapping input="@../instrumentState0" output="@instrumentState"/>
			<Node name="Grasper">
				<MechanicalObject name="Articulations" template="Vec1d" size="8" tags="R" position="@innerInstrumentSwitch.posture" rest_position="@innerInstrumentSwitch.posture"/>
				<UniformMass name="mass" totalMass="0.5" />
				<RestShapeSpringsForceField name="artiSprings" points="0 1 2 3 4 5 6 7" stiffness="11 11 100 100 100 100 100 100" angularStiffness="1e2 1e2 100 100 100 100 100 100" />
				<LCPForceFeedback name="ffb" activate="true" forceCoef="0.001"/> 
				<UncoupledConstraintCorrection compliance="0.0005 0.0005 0.01 0 0.01 0 0.005 0"/>
				<Node name="Jaws">
					<MechanicalObject name="Bones" template="Rigid3d" size="9" position="0 0 0 0 0 0 1 
																						 0 0 0 0 0 0 1
																						 0 0 0 0 0 0 1
																						 0 0 0 0 0 0 1
																						 0 0 0 0 0 0 1
																						 0 0 0 0 0 0 1
																						 0 0 0 0 0 0 1
																						 0 0 0 0 0 0 1
																						 0 0 0 0 0 0 1" showObject="0" showObjectScale="1"/>
					<Node name="EndoscopeVisual">
						<OglModel name="Visual" filename="endoscope.obj" scale="0.1"/>
						<RigidMapping index="4"/>
					</Node>
					<Node name="TubeVisual">
						<OglModel name="Visual" filename="tube.obj" scale="0.1"/>
						<RigidMapping index="6"/>
					</Node>
					<Node name ="TubeCollision" tags="secondDistance">
						<MeshObjLoader filename="tubeCollision0.obj"  name="loader"/>
						<Mesh src="@loader" />
						<MechanicalObject src="@loader" scale="0.1" dz="-0.0"/>
						<Line group="1" contactStiffness="1e1" />
						<Point group="1" contactStiffness="1e1" />
						<RigidMapping index="6"/>
					</Node>
					<Node name="Endoscope">
						<MechanicalObject name="endoscope" template="Rigid3d" position="0 0 0 0 0 0 1" showObject="0" showObjectScale="1" tags="endoscopeState"/>
						<RigidRigidMapping index="4"/>
					</Node>
					<Node name="Tube">
						<MechanicalObject name="tube" template="Rigid3d" position="0 0 0 0 0 0 1" showObject="0" showObjectScale="1" tags="tubeState"/>
						<RigidRigidMapping index="6"/>
					</Node>
					<Node name="InnerInstrument">
						<MechanicalObject name="innerInstrument" template="Rigid3d" position="0 0 0 0 0 0 1" showObject="1" showObjectScale="1" tags="proxyState"/>
						<RigidRigidMapping index="8"/>
					</Node>
					<Node name="Point">
						<MechanicalObject name="ipoint" position="0 0 0 0 0 0 1" showObject="0" showObjectScale="10" tags="instrumentPoint"/>
						<RigidMapping index="8"/>
					</Node>
					<ArticulatedSystemMapping input1="@../Articulations" output="@Bones" input2="@../../instrumentState"/>
				</Node>
				
				<ArticulatedHierarchyContainer/>
				<Node name="articulationCenters">
					<Node name="articulationCenter0">
						<ArticulationCenter parentIndex="0" childIndex="1" posOnParent="0 0 0" posOnChild="0 0 0"/>
						<Node name="articulations">
							<Articulation translation="0" rotation="1" axis="0 1 0" articulationIndex="0"/>
						</Node>
					</Node>
					<Node name="articulationCenter1">
						<ArticulationCenter parentIndex="1" childIndex="2" posOnParent="0 0 0" posOnChild="0 0 0"/>
						<Node name="articulations">
							<Articulation translation="0" rotation="1" axis="1 0 0" articulationIndex="1"/>
						</Node>
					</Node>
					<Node name="articulationCenter2">
						<ArticulationCenter parentIndex="2" childIndex="3" posOnParent="0 0 0" posOnChild="0 0 0"/>
						<Node name="articulations">
							<Articulation translation="1" rotation="0" axis="0 0 1" articulationIndex="2"/>
						</Node>
					</Node>
					<Node name="articulationCenter3">
						<ArticulationCenter parentIndex="3" childIndex="4" posOnParent="0 0 0" posOnChild="0 0 0"/>
						<Node name="articulations">
							<Articulation translation="0" rotation="1" axis="0 0 1" articulationIndex="3"/>
						</Node>
					</Node>
					<Node name="articulationCenter4">
						<ArticulationCenter parentIndex="2" childIndex="5" posOnParent="0 0 0" posOnChild="0 0 0"/>
						<Node name="articulations">
							<Articulation translation="1" rotation="0" axis="0 0 1" articulationIndex="4"/>
						</Node>
					</Node>
					<Node name="articulationCenter5">
						<ArticulationCenter parentIndex="5" childIndex="6" posOnParent="0 0 0" posOnChild="0 0 0"/>
						<Node name="articulations">
							<Articulation translation="0" rotation="1" axis="0 0 1" articulationIndex="5"/>
						</Node>
					</Node>
					<Node name="articulationCenter6">
						<ArticulationCenter parentIndex="2" childIndex="7" posOnParent="0 0 0" posOnChild="0 0 0"/>
						<Node name="articulations">
							<Articulation translation="1" rotation="0" axis="0 0 1" articulationIndex="6"/>
						</Node>
					</Node>
					<Node name="articulationCenter7">
						<ArticulationCenter parentIndex="7" childIndex="8" posOnParent="0 0 0" posOnChild="0 0 0"/>
						<Node name="articulations">
							<Articulation translation="0" rotation="1" axis="0 0 1" articulationIndex="7"/>
						</Node>
					</Node>
				</Node>
				
			</Node>
		</Node>
		
	</Node>
	
	<Node name="RestPoint" >
		<EulerImplicitSolver name="ODE solver" rayleighStiffness="0.05" rayleighMass="1.0" />
        <CGLinearSolver name="linear solver" iterations="25" tolerance="1e-10" threshold="10e-10" /> 
        <MechanicalObject name="rpoint" size="1" position="2.73 -10.23 8.94" showObject="1" showObjectScale="10" tags="restPoint"/>
        <UniformMass vertexMass="0.1" />
        <RestShapeSpringsForceField name="Springs" points="0" stiffness="100000"/>
		<PrecomputedConstraintCorrection recompute="true"/>
	</Node>	
	<BilateralInteractionConstraint name="bilateral" template="Vec3d" object1="@RestPoint/rpoint" object2="@Instrument/Mapping/Grasper/Jaws/Point/ipoint" first_point="" second_point="" />
</Node>

```
{collapsible="true" collapsed-title="Main.scn"}

SOFA的.scn文件是一种使用XML标记来描述物理模拟场景的文件。在这个文件中，描述了一个使用多种不同元素和交互的复杂场景。下面是一些主要元素的解释：

- `<Node>`: 这是场景图的基本元素，它可以包含其他节点或者组件。

- `<AddDataRepository>`: 这个标签是用来添加数据存储库的路径，这样SOFA就可以在这个路径下查找需要的资源，如网格模型等。

- `<RequiredPlugin>`: 这个标签用于加载SOFA插件。在这里，加载了`SofaOpenglVisual`，`SofaHaptics`和`MultiDistanceContact`等插件。

- `<VisualStyle>`: 这个标签用于设置视觉样式，例如隐藏行为模型、碰撞模型、映射和力场。

- `<DefaultPipeline>`、`<BruteForceDetection>`、`<DefaultContactManager>`、`<MultiLocalMinDistance>`、`<FreeMotionAnimationLoop>`和`<GenericConstraintSolver>`: 这些都是物理模拟的重要环节，包括碰撞检测，碰撞响应，距离测量，动画循环和约束求解器。

- `<InnerInstrumentSwitch>`: 这可能是一个自定义节点，描述了内部工具的状态，例如位置和末端镜的状态。

- `<Node name="Spine" >`: 这个节点可能代表脊柱模型，它内部包含了视觉模型和碰撞模型，分别由`<OglModel>`和`<MeshObjLoader>`等标签定义。

- `<Node name="Instrument" >`: 这个节点可能代表一个手术器械或者工具，它包含有关物理属性（如施加的力和质量）的信息，以及视觉和碰撞模型。它还包含了一个复杂的子节点结构，用于处理各种工具和部件的交互。

- `<!-- -->`: 这是注释标签，被注释的部分在SOFA加载.scn文件时，不会被解析和加载。

这只是一个大概的解释，具体的细节和含义可能需要根据具体的SOFA版本和使用的插件来确定。

```xml
<InnerInstrumentSwitch name="innerInstrumentSwitch" 
                       positionAdd="-10.0 4.9 -0.9 0 -0.707107 0 0.707107" 
                       zEndoscopeAdd = "7.0" 
                       zTubeAdd = "7.0"/>
```
这里定义了支点,以及那个四元数.以及endoscope和tube的偏移,so他会跟随谁动?

```xml
<MechanicalObject name="instrumentState0" 
                  template="Rigid3d" 
                  position="@innerInstrumentSwitch.positionAdd" 
                  showObject="1" 
                  showObjectScale="2" 
                  tags="instrument0"/>
```
这里说了,position来自`innerInstrumentSwitch.positionAdd`,这是SOFA里定义的初始位置啊,怎么驱动?

```xml
<MechanicalObject name="Articulations" 
                  template="Vec1d" 
                  size="8" 
                  tags="R" 
                  position="@innerInstrumentSwitch.posture" 
                  rest_position="@innerInstrumentSwitch.posture"/>
```
这个是Grasper,他的位置来自`innerInstrumentSwitch.posture`,可是posture在scn里有定义吗?没有,所以是在代码里?

```C++
      if (sofa_scene.get() && sofa_scene->sofa_instrument_switch.get()) {
        sofa::defaulttype::Vec1Types::VecCoord &ps =
            *sofa_scene->sofa_instrument_switch->d_posture.beginEdit();
        ComputePositions(translationDevice, orientationDevice, ps);
        sofa_scene->sofa_instrument_switch->d_posture.endEdit();
      }
```
这个是钳子的姿态?


## ODE Solver {collapsible="true"}
`ODE solver`，全称是 Ordinary Differential Equations solver，即常微分方程求解器。在物理模拟中，常微分方程求解器用于数值解决物体运动的方程，这些方程描述了物体的位置、速度和加速度如何随时间变化。

在这个 `.scn` 文件中，`EulerImplicitSolver` 是一种 `ODE solver`：

```xml
<EulerImplicitSolver name="ODE solver" rayleighStiffness="0.05" rayleighMass="1.0" />
```

`EulerImplicitSolver` 是基于欧拉方法的隐式求解器。相较于显式方法，隐式求解器在处理具有高刚度（即物体在受到位移后会快速恢复原位）的系统时更稳定。但是，它需要解决更大的线性系统，因此计算成本可能更高。

在这个 `EulerImplicitSolver` 的定义中，`name="ODE solver"` 是这个求解器的名称。`rayleighStiffness="0.05"` 和 `rayleighMass="1.0"` 是雷利阻尼的参数，雷利阻尼是一种模拟阻尼（即能量损失）的方法，它可以帮助稳定模拟。

总的来说，`ODE solver` 在模拟中起着核心的作用，它负责根据物体的力和质量计算物体的运动。


`CGLinearSolver` 是 SOFA 中的一个组件，用于解决线性方程系统。在物理模拟中，线性方程系统经常出现，例如在求解刚体动力学或者弹性物体动力学时。

在这个 `.scn` 文件中，`CGLinearSolver` 的定义如下：

```xml
<CGLinearSolver name="linear solver" iterations="25" tolerance="1e-09" threshold="1e-09"/>
```

这个定义中的各个属性含义如下：

- `name="linear solver"`：这是求解器的名称，在场景图中可以用它来唯一标识这个求解器。

- `iterations="25"`：这是求解器在停止之前可以执行的最大迭代次数。迭代次数越高，求解的精度可能越高，但计算成本也越大。

- `tolerance="1e-09"` 和 `threshold="1e-09"`：这两个参数控制了求解器的停止条件。当解的改进（相对于前一次迭代）小于 `tolerance`，或者解的残差小于 `threshold` 时，求解器会停止迭代。

`CG` 在 `CGLinearSolver` 中代表共轭梯度法，这是一种迭代方法，用于求解线性方程组。共轭梯度法特别适合于求解大规模的稀疏线性方程组，这种方程组在物理模拟中经常出现。



