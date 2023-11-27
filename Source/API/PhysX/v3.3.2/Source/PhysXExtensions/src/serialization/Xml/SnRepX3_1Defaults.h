// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2014 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

	DEFINE_REPX_DEFAULT_PROPERTY("PxTolerancesScale.Length", "1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxTolerancesScale.Mass", "1000" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxTolerancesScale.Speed", "10" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxBoxGeometry.HalfExtents", "0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphereGeometry.Radius", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxConvexMeshGeometry.Scale.Scale", "1 1 1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxConvexMeshGeometry.Scale.Rotation", "0 0 0 1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxConvexMeshGeometry.ConvexMesh", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxTriangleMeshGeometry.Scale.Scale", "1 1 1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxTriangleMeshGeometry.Scale.Rotation", "0 0 0 1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxTriangleMeshGeometry.MeshFlags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxTriangleMeshGeometry.TriangleMesh", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxHeightFieldGeometry.HeightField", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxHeightFieldGeometry.HeightScale", "1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxHeightFieldGeometry.RowScale", "1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxHeightFieldGeometry.ColumnScale", "1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxHeightFieldGeometry.HeightFieldFlags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxMaterial.DynamicFriction", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxMaterial.StaticFriction", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxMaterial.Restitution", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxMaterial.DynamicFrictionV", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxMaterial.StaticFrictionV", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxMaterial.DirOfAnisotropy", "1 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxMaterial.Flags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxMaterial.FrictionCombineMode", "eAVERAGE" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxMaterial.RestitutionCombineMode", "eAVERAGE" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxShape.LocalPose", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxShape.SimulationFilterData", "0 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxShape.QueryFilterData", "0 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxShape.ContactOffset", "0.02" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxShape.RestOffset", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxShape.Flags", "eSIMULATION_SHAPE|eSCENE_QUERY_SHAPE|eVISUALIZATION" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxShape.Name", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidStatic.Name", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidStatic.ActorFlags", "eVISUALIZATION" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidStatic.DominanceGroup", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidStatic.OwnerClient", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidStatic.ClientBehaviorBits", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidStatic.GlobalPose", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.Name", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.ActorFlags", "eVISUALIZATION" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.DominanceGroup", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.OwnerClient", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.ClientBehaviorBits", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.GlobalPose", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.CMassLocalPose", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.Mass", "1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.MassSpaceInertiaTensor", "1 1 1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.LinearVelocity", "0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.AngularVelocity", "0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.LinearDamping", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.AngularDamping", "0.05" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.MaxAngularVelocity", "7" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.SleepThreshold", "0.005" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.SolverIterationCounts.minPositionIters", "4" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.SolverIterationCounts.minVelocityIters", "1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.ContactReportThreshold", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRigidDynamic.RigidDynamicFlags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.ParentPose", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.ChildPose", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.TargetOrientation", "0 0 0 1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.TargetVelocity", "0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.Spring", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.Damping", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.InternalCompliance", "1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.ExternalCompliance", "1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.SwingLimit.yLimit", "0.78539816339" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.SwingLimit.zLimit", "0.78539816339" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.TangentialSpring", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.TangentialDamping", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.SwingLimitContactDistance", "0.05" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.SwingLimitEnabled", "false" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.TwistLimit.lower", "-0.78539816339" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.TwistLimit.upper", "0.78539816339" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.TwistLimitEnabled", "false" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationJoint.TwistLimitContactDistance", "0.05" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationLink.Name", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationLink.ActorFlags", "eVISUALIZATION" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationLink.DominanceGroup", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationLink.OwnerClient", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationLink.ClientBehaviorBits", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationLink.GlobalPose", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationLink.CMassLocalPose", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationLink.Mass", "1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationLink.MassSpaceInertiaTensor", "1 1 1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationLink.LinearVelocity", "0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulationLink.AngularVelocity", "0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulation.MaxProjectionIterations", "4" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulation.SeparationTolerance", "0.1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulation.InternalDriveIterations", "4" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulation.ExternalDriveIterations", "4" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulation.SolverIterationCounts.minPositionIters", "4" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulation.SolverIterationCounts.minVelocityIters", "1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulation.SleepThreshold", "0.005" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxArticulation.Name", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Actors.actor0", "8887040" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Actors.actor1", "8887456" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.LocalPose.eACTOR0", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.LocalPose.eACTOR1", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.BreakForce.force", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.BreakForce.torque", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.ConstraintFlags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Name", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Motion.eX", "eLOCKED" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Motion.eY", "eLOCKED" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Motion.eZ", "eLOCKED" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Motion.eTWIST", "eLOCKED" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Motion.eSWING1", "eLOCKED" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Motion.eSWING2", "eLOCKED" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.LinearLimit.Restitution", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.LinearLimit.Spring", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.LinearLimit.Damping", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.LinearLimit.ContactDistance", "0.05" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.LinearLimit.Value", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.TwistLimit.Restitution", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.TwistLimit.Spring", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.TwistLimit.Damping", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.TwistLimit.ContactDistance", "0.05" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.TwistLimit.Upper", "1.5708" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.TwistLimit.Lower", "-1.5708" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.SwingLimit.Restitution", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.SwingLimit.Spring", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.SwingLimit.Damping", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.SwingLimit.ContactDistance", "0.05" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.SwingLimit.YAngle", "1.5708" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.SwingLimit.ZAngle", "1.5708" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eX.Spring", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eX.Damping", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eX.ForceLimit", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eX.Flags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eY.Spring", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eY.Damping", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eY.ForceLimit", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eY.Flags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eZ.Spring", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eZ.Damping", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eZ.ForceLimit", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eZ.Flags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eSWING.Spring", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eSWING.Damping", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eSWING.ForceLimit", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eSWING.Flags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eTWIST.Spring", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eTWIST.Damping", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eTWIST.ForceLimit", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eTWIST.Flags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eSLERP.Spring", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eSLERP.Damping", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eSLERP.ForceLimit", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.Drive.eSLERP.Flags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.DrivePosition", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.DriveVelocity.linear", "0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.DriveVelocity.angular", "0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.ProjectionLinearTolerance", "1e+010" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxD6Joint.ProjectionAngularTolerance", "3.14159" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxFixedJoint.Actors.actor0", "8887040" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxFixedJoint.Actors.actor1", "8887456" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxFixedJoint.LocalPose.eACTOR0", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxFixedJoint.LocalPose.eACTOR1", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxFixedJoint.BreakForce.force", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxFixedJoint.BreakForce.torque", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxFixedJoint.ConstraintFlags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxFixedJoint.Name", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxFixedJoint.ProjectionLinearTolerance", "1e+010" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxFixedJoint.ProjectionAngularTolerance", "3.14159" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxDistanceJoint.Actors.actor0", "8887040" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxDistanceJoint.Actors.actor1", "8887456" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxDistanceJoint.LocalPose.eACTOR0", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxDistanceJoint.LocalPose.eACTOR1", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxDistanceJoint.BreakForce.force", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxDistanceJoint.BreakForce.torque", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxDistanceJoint.ConstraintFlags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxDistanceJoint.Name", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxDistanceJoint.MinDistance", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxDistanceJoint.MaxDistance", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxDistanceJoint.Tolerance", "0.025" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxDistanceJoint.Spring", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxDistanceJoint.Damping", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxDistanceJoint.DistanceJointFlags", "eMAX_DISTANCE_ENABLED" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.Actors.actor0", "8887040" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.Actors.actor1", "8887456" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.LocalPose.eACTOR0", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.LocalPose.eACTOR1", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.BreakForce.force", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.BreakForce.torque", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.ConstraintFlags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.Name", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.Limit.Restitution", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.Limit.Spring", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.Limit.Damping", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.Limit.ContactDistance", "0.05" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.Limit.Upper", "1.5708" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.Limit.Lower", "-1.5708" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.DriveVelocity", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.DriveForceLimit", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.DriveGearRatio", "1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.RevoluteJointFlags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.ProjectionLinearTolerance", "1e+010" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxRevoluteJoint.ProjectionAngularTolerance", "3.14159" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.Actors.actor0", "8887040" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.Actors.actor1", "8887456" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.LocalPose.eACTOR0", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.LocalPose.eACTOR1", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.BreakForce.force", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.BreakForce.torque", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.ConstraintFlags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.Name", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.Limit.Restitution", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.Limit.Spring", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.Limit.Damping", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.Limit.ContactDistance", "0.01" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.Limit.Upper", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.Limit.Lower", "-3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.PrismaticJointFlags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.ProjectionLinearTolerance", "1e+010" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxPrismaticJoint.ProjectionAngularTolerance", "3.14159" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphericalJoint.Actors.actor0", "8887040" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphericalJoint.Actors.actor1", "8887456" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphericalJoint.LocalPose.eACTOR0", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphericalJoint.LocalPose.eACTOR1", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphericalJoint.BreakForce.force", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphericalJoint.BreakForce.torque", "3.40282e+038" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphericalJoint.ConstraintFlags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphericalJoint.Name", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphericalJoint.LimitCone.Restitution", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphericalJoint.LimitCone.Spring", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphericalJoint.LimitCone.Damping", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphericalJoint.LimitCone.ContactDistance", "0.05" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphericalJoint.LimitCone.YAngle", "1.5708" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphericalJoint.LimitCone.ZAngle", "1.5708" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphericalJoint.SphericalJointFlags", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxSphericalJoint.ProjectionLinearTolerance", "1e+010" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxCloth.Name", "" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxCloth.ActorFlags", "eVISUALIZATION" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxCloth.DominanceGroup", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxCloth.OwnerClient", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxCloth.ClientBehaviorBits", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxCloth.MotionConstraintScaleBias.scale", "1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxCloth.MotionConstraintScaleBias.bias", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxCloth.GlobalPose", "0 0 0 1 0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxCloth.ExternalAcceleration", "0 0 0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxCloth.DampingCoefficient", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxCloth.SolverFrequency", "1" )
	DEFINE_REPX_DEFAULT_PROPERTY("PxCloth.SleepLinearVelocity", "0" )
	DEFINE_REPX_DEFAULT_PROPERTY("THEEND", "false" )
