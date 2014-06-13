' Copyright 2013, Gurobi Optimization, Inc.
'
' Use parameters that are associated with a model.

' A MIP is solved for 5 seconds with different sets of parameters.
' The one with the smallest MIP gap is selected, and the optimization
' is resumed until the optimal solution is found.

Imports System
Imports Gurobi

Class params_vb
    ' Simple function to determine the MIP gap
    Private Shared Function Gap(ByVal model As GRBModel) As Double
        If (model.Get(GRB.IntAttr.SolCount) = 0) OrElse _
           (Math.Abs(model.Get(GRB.DoubleAttr.ObjVal)) < 0.000001) Then
            Return GRB.INFINITY
        End If
        Return Math.Abs(model.Get(GRB.DoubleAttr.ObjBound) - _
                        model.Get(GRB.DoubleAttr.ObjVal)) / _
                   Math.Abs(model.Get(GRB.DoubleAttr.ObjVal))
    End Function

    Shared Sub Main(ByVal args As String())

        If args.Length < 1 Then
            Console.WriteLine("Usage: params_vb filename")
            Return
        End If

        Try
            ' Read model and verify that it is a MIP
            Dim env As New GRBEnv()
            Dim basemodel As New GRBModel(env, args(0))
            If basemodel.Get(GRB.IntAttr.IsMIP) = 0 Then
                Console.WriteLine("The model is not an integer program")
                Environment.Exit(1)
            End If

            ' Set a 5 second time limit
            basemodel.GetEnv().Set(GRB.DoubleParam.TimeLimit, 5)

            ' Now solve the model with different values of MIPFocus
            Dim bestGap As Double = GRB.INFINITY
            Dim bestModel As GRBModel = Nothing
            For i As Integer = 0 To 3
                Dim m As New GRBModel(basemodel)
                m.GetEnv().Set(GRB.IntParam.MIPFocus, i)
                m.Optimize()
                If bestModel Is Nothing OrElse bestGap > Gap(m) Then
                    bestModel = m
                    bestGap = Gap(bestModel)
                End If
            Next

            ' Finally, reset the time limit and continue to solve the
            ' best model to optimality
            bestModel.GetEnv().Set(GRB.DoubleParam.TimeLimit, GRB.INFINITY)
            bestModel.Optimize()

            Console.WriteLine("Solved with MIPFocus: " & _
                              bestModel.GetEnv().Get(GRB.IntParam.MIPFocus))
        Catch e As GRBException
            Console.WriteLine("Error code: " & e.ErrorCode & ". " & e.Message)
        End Try
    End Sub
End Class
