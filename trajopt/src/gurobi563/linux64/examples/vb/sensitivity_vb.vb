' Copyright 2013, Gurobi Optimization, Inc.
'
' Simple MIP sensitivity analysis example.
' For each integer variable, fix it to its lower and upper bound
' and check the impact on the objective.

Imports System
Imports Gurobi

Class sensitivity_vb
    Shared Sub Main(ByVal args As String())

        If args.Length < 1 Then
            Console.WriteLine("Usage: sensitivity_vb filename")
            Return
        End If

        Try
            ' Read model
            Dim env As New GRBEnv()
            Dim a As New GRBModel(env, args(0))
            a.Optimize()
            a.GetEnv().Set(GRB.IntParam.OutputFlag, 0)

            ' Extract variables from model
            Dim avars As GRBVar() = a.GetVars()

            For i As Integer = 0 To avars.Length - 1
                Dim v As GRBVar = avars(i)
                If v.Get(GRB.CharAttr.VType) = GRB.BINARY Then

                    ' Create clone and fix variable
                    Dim b As New GRBModel(a)
                    Dim bv As GRBVar = b.GetVars()(i)
                    If v.Get(GRB.DoubleAttr.X) - v.Get(GRB.DoubleAttr.LB) < 0.5 Then
                        bv.Set(GRB.DoubleAttr.LB, bv.Get(GRB.DoubleAttr.UB))
                    Else
                        bv.Set(GRB.DoubleAttr.UB, bv.Get(GRB.DoubleAttr.LB))
                    End If

                    b.Optimize()

                    If b.Get(GRB.IntAttr.Status) = GRB.Status.OPTIMAL Then
                        Dim objchg As Double = b.Get(GRB.DoubleAttr.ObjVal) - _
                                               a.Get(GRB.DoubleAttr.ObjVal)
                        If objchg < 0 Then
                            objchg = 0
                        End If
                        Console.WriteLine("Objective sensitivity for variable " & _
                                          v.Get(GRB.StringAttr.VarName) & _
                                          " is " & objchg)
                    Else
                        Console.WriteLine("Objective sensitivity for variable " & _
                                          v.Get(GRB.StringAttr.VarName) & _
                                          " is infinite")
                    End If

                    ' Dispose of model
                    b.Dispose()
                End If
            Next

            ' Dispose of model and env
            a.Dispose()
            env.Dispose()

        Catch e As GRBException
            Console.WriteLine("Error code: " & e.ErrorCode & ". " + e.Message)
        End Try
    End Sub
End Class
