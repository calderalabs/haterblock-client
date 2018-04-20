open Belt;

[%bs.raw {|require('./CommentListFilters.css')|}];

let component = ReasonReact.statelessComponent("CommentListFilters");

type filters = {
  status: list(CommentData.Status.t),
  sentiment: list(CommentData.Sentiment.t),
};

let make = (~filters: filters, ~onFiltersChange: filters => unit, _children) => {
  let isStatusChecked = (status: CommentData.Status.t) =>
    filters.status |> List.has(_, status, (==));
  let onToggleStatus = (status: CommentData.Status.t) => {
    let newStatus =
      if (isStatusChecked(status)) {
        filters.status |> List.keep(_, s => s != status);
      } else {
        filters.status |> List.add(_, status);
      };
    onFiltersChange({...filters, status: newStatus});
  };
  let isSentimentChecked = (sentiment: CommentData.Sentiment.t) =>
    filters.sentiment |> List.has(_, sentiment, (==));
  let onToggleSentiment = (sentiment: CommentData.Sentiment.t) => {
    let newSentiment =
      if (isSentimentChecked(sentiment)) {
        filters.sentiment |> List.keep(_, s => s != sentiment);
      } else {
        filters.sentiment |> List.add(_, sentiment);
      };
    onFiltersChange({...filters, sentiment: newSentiment});
  };
  {
    ...component,
    render: _self =>
      <div className="CommentListFilters">
        <div className="CommentListFilters__filter">
          <Checkbox
            label=(CommentData.Status.toString(CommentData.Status.Published))
            checked=(isStatusChecked(CommentData.Status.Published))
            onChange=(() => onToggleStatus(CommentData.Status.Published))
            className="Checkbox--small"
          />
        </div>
        <div className="CommentListFilters__filter">
          <Checkbox
            label=(CommentData.Status.toString(CommentData.Status.Rejected))
            checked=(isStatusChecked(CommentData.Status.Rejected))
            onChange=(() => onToggleStatus(CommentData.Status.Rejected))
            className="Checkbox--small"
          />
        </div>
        <div className="CommentListFilters__filter">
          <Checkbox
            label=(
              CommentData.Sentiment.toString(CommentData.Sentiment.Hateful)
            )
            checked=(isSentimentChecked(CommentData.Sentiment.Hateful))
            onChange=(() => onToggleSentiment(CommentData.Sentiment.Hateful))
            className="Checkbox--small"
          />
        </div>
        <div className="CommentListFilters__filter">
          <Checkbox
            label=(
              CommentData.Sentiment.toString(CommentData.Sentiment.Negative)
            )
            checked=(isSentimentChecked(CommentData.Sentiment.Negative))
            onChange=(() => onToggleSentiment(CommentData.Sentiment.Negative))
            className="Checkbox--small"
          />
        </div>
        <div className="CommentListFilters__filter">
          <Checkbox
            label=(
              CommentData.Sentiment.toString(CommentData.Sentiment.Neutral)
            )
            checked=(isSentimentChecked(CommentData.Sentiment.Neutral))
            onChange=(() => onToggleSentiment(CommentData.Sentiment.Neutral))
            className="Checkbox--small"
          />
        </div>
        <div className="CommentListFilters__filter">
          <Checkbox
            label=(
              CommentData.Sentiment.toString(CommentData.Sentiment.Positive)
            )
            checked=(isSentimentChecked(CommentData.Sentiment.Positive))
            onChange=(() => onToggleSentiment(CommentData.Sentiment.Positive))
            className="Checkbox--small"
          />
        </div>
      </div>,
  };
};
